#pragma once

#include "assert.hpp"
#include "macros.hpp"

#include <functional>
#include <vector>
#include <map>

#define PP_CREATE_SCOPED_OBJ(Klass, Name, ...) \
UNUSED(Klass PP_UNIQUE_LABEL(scoped_##Name)(__VA_ARGS__))


#define REQUIRES(expr) \
PP_CREATE_SCOPED_OBJ(contract::InvariantScope<std::remove_pointer_t<decltype(this)>>, invariant, this, __func__);	\
PP_CREATE_SCOPED_OBJ(contract::RequireContract, contract, [&]() { ASSERT(expr); })

#define ENSURES(expr) \
PP_CREATE_SCOPED_OBJ(contract::InvariantScope<std::remove_pointer_t<decltype(this)>>, invariant, this, __func__);	\
PP_CREATE_SCOPED_OBJ(contract::EnsureContract, contract, [&]() { ASSERT(expr); })

#define REQUIRES_P(expr) \
PP_CREATE_SCOPED_OBJ(contract::RequireContract, contract, [&]() { ASSERT(expr); })

#define ENSURES_P(expr) \
PP_CREATE_SCOPED_OBJ(contract::EnsureContract, contract, [&]() { ASSERT(expr); })

#define TEST_INVARIANTS \
PP_CREATE_SCOPED_OBJ(contract::InvariantScope<std::remove_pointer<decltype(this)>::type>, invariant, this, __func__);

#define BEGIN_INVARIANT(Klass)						\
friend struct contract::InvariantContractTester;	\
struct InvariantContractHolder						\
{													\
static void doTest(const Klass& self) {

#define END_INVARIANT()	\
}};

#define INVARIANT(expr) \
ASSERT(expr);

namespace contract
{
	struct InvariantContractTester
	{
		template<class ActualKlass>
		inline static void performTests(const ActualKlass* self)
		{
			if constexpr(std::is_function<decltype(ActualKlass::InvariantContractHolder::doTest)>::value)
			{
				ActualKlass::InvariantContractHolder::doTest(*self);	
			}
		}
	};

	struct RequireContract
	{
		template<class Function>
		inline RequireContract(Function&& func)
		{
			func();
		}
	};

	struct EnsureContract
	{
		template<class Function>
		inline EnsureContract(Function&& func)
		{
			myFunction = func;
		}

		inline ~EnsureContract()
		{
			myFunction();
		}

		std::function<void()> myFunction = nullptr;
	};

	template<class Klass>
	struct InvariantScope
	{
		InvariantScope(const Klass* self, const char* function)
		{
			mySelf = self;
			myFunction = function;

			if(ourCounter[myFunction] == 0)
				contract::InvariantContractTester::performTests(mySelf);

			ourCounter[myFunction]++;
		}

		~InvariantScope()
		{
			ourCounter[myFunction]--;

			if(ourCounter[myFunction] == 0)
				contract::InvariantContractTester::performTests(mySelf);
		}

		static std::map<const char*, int> ourCounter;
		const Klass* mySelf;
		const char* myFunction;
	};

	template<class Klass>
	std::map<const char*, int> InvariantScope<Klass>::ourCounter;
}
