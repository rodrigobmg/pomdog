//======================================================================
//-----------------------------------------------------------------------
/**
 * @file		iutest_pred.hpp
 * @brief		iris unit test �q�� Assertion �t�@�C��
 *
 * @author		t.sirayanagi
 * @version		1.0
 *
 * @par			copyright
 * Copyright (C) 2011-2012, Takazumi Shirayanagi\n
 * This software is released under the new BSD License,
 * see LICENSE
*/
//-----------------------------------------------------------------------
//======================================================================
#ifndef INCG_IRIS_iutest_pred_HPP_ADF851F3_C89B_4ca4_B9A0_A2B2229945C6_
#define INCG_IRIS_iutest_pred_HPP_ADF851F3_C89B_4ca4_B9A0_A2B2229945C6_

//======================================================================
// include
#include "iutest_assertion.hpp"

//======================================================================
// define

namespace iutest
{

// 1
/** @private */
template<typename PRED, typename T1>
AssertionResult	AssertPred1Helper(const char* pred_str, const char* expr1
							  , PRED pred, T1 val1)
{
	if( (*pred)(val1) ) return AssertionSuccess();
	return AssertionFailure() << "error: " << pred_str << "(" << expr1 << ") evaluates to false, where "
		<< "\n" << expr1 << " : " << val1 ;
}

/**
 * @private
 * @{
*/
#define IUTEST_PRED1_(pred, v1, on_failure)	\
	IUTEST_TEST_ASSERT_(::iutest::AssertPred1Helper(#pred, #v1, pred, v1), on_failure)
#define IUTEST_PRED_FORMAT1_(pred_formatter, v1, on_failure)	\
	IUTEST_TEST_ASSERT_(pred_formatter(#v1, v1), on_failure)
/**
 * @}
*/

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSERT_PRED1(pred, v1)		\
	IUTEST_PRED1_(pred, v1, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_EXPECT_PRED1(pred, v1)		\
	IUTEST_PRED1_(pred, v1, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_INFORM_PRED1(pred, v1)		\
	IUTEST_PRED1_(pred, v1, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSUME_PRED1(pred, v1)		\
	IUTEST_PRED1_(pred, v1, IUTEST_ASSUME_FAILURE)

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSERT_PRED_FORMAT1(pred_formatter, v1)		\
	IUTEST_PRED_FORMAT1_(pred_formatter, v1, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_EXPECT_PRED_FORMAT1(pred_formatter, v1)		\
	IUTEST_PRED_FORMAT1_(pred_formatter, v1, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_INFORM_PRED_FORMAT1(pred_formatter, v1)		\
	IUTEST_PRED_FORMAT1_(pred_formatter, v1, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSUME_PRED_FORMAT1(pred_formatter, v1)		\
	IUTEST_PRED_FORMAT1_(pred_formatter, v1, IUTEST_ASSUME_FAILURE)


// 2
/** @private */
template<typename PRED, typename T1, typename T2>
AssertionResult	AssertPred2Helper(const char* pred_str, const char* expr1, const char* expr2
							  , PRED pred, T1 val1, T2 val2)
{
	if( (*pred)(val1, val2) ) return AssertionSuccess();
	return AssertionFailure() << "error: " << pred_str << "("
		<< expr1 << ", " << expr2
		<< ") evaluates to false, where "
		<< "\n" << expr1 << " : " << val1
		<< "\n" << expr2 << " : " << val2 ;
}

/**
 * @private
 * @{
*/
#define IUTEST_PRED2_(pred, v1, v2, on_failure)	\
	IUTEST_TEST_ASSERT_(::iutest::AssertPred2Helper(#pred, #v1, #v2, pred, v1, v2), on_failure)
#define IUTEST_PRED_FORMAT2_(pred_formatter, v1, v2, on_failure)	\
	IUTEST_TEST_ASSERT_(pred_formatter(#v1, #v2, v1, v2), on_failure)
/**
 * @}
*/

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSERT_PRED2(pred, v1, v2)		\
	IUTEST_PRED2_(pred, v1, v2, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_EXPECT_PRED2(pred, v1, v2)		\
	IUTEST_PRED2_(pred, v1, v2, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_INFORM_PRED2(pred, v1, v2)		\
	IUTEST_PRED2_(pred, v1, v2, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSUME_PRED2(pred, v1, v2)		\
	IUTEST_PRED2_(pred, v1, v2, IUTEST_ASSUME_FAILURE)

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSERT_PRED_FORMAT2(pred_formatter, v1, v2)		\
	IUTEST_PRED_FORMAT2_(pred_formatter, v1, v2, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_EXPECT_PRED_FORMAT2(pred_formatter, v1, v2)		\
	IUTEST_PRED_FORMAT2_(pred_formatter, v1, v2, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_INFORM_PRED_FORMAT2(pred_formatter, v1, v2)		\
	IUTEST_PRED_FORMAT2_(pred_formatter, v1, v2, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSUME_PRED_FORMAT2(pred_formatter, v1, v2)		\
	IUTEST_PRED_FORMAT2_(pred_formatter, v1, v2, IUTEST_ASSUME_FAILURE)

// 3
/** @private */
template<typename PRED, typename T1, typename T2, typename T3>
AssertionResult	AssertPred3Helper(const char* pred_str, const char* expr1, const char* expr2, const char* expr3
							  , PRED pred, T1 val1, T2 val2, T3 val3)
{
	if( (*pred)(val1, val2, val3) ) return AssertionSuccess();
	return AssertionFailure() << "error: " << pred_str << "("
		<< expr1 << ", " << expr2 << ", " << expr3
		<< ") evaluates to false, where "
		<< "\n" << expr1 << " : " << val1
		<< "\n" << expr2 << " : " << val2
		<< "\n" << expr3 << " : " << val3 ;
}

/**
 * @private
 * @{
*/
#define IUTEST_PRED3_(pred, v1, v2, v3, on_failure)	\
	IUTEST_TEST_ASSERT_(::iutest::AssertPred3Helper(#pred, #v1, #v2, #v3, pred, v1, v2, v3), on_failure)
#define IUTEST_PRED_FORMAT3_(pred_formatter, v1, v2, v3, on_failure)	\
	IUTEST_TEST_ASSERT_(pred_formatter(#v1, #v2, #v3, v1, v2, v3), on_failure)
/**
 * @}
*/

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSERT_PRED3(pred, v1, v2, v3)		\
	IUTEST_PRED3_(pred, v1, v2, v3, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_EXPECT_PRED3(pred, v1, v2, v3)		\
	IUTEST_PRED3_(pred, v1, v2, v3, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_INFORM_PRED3(pred, v1, v2, v3)		\
	IUTEST_PRED3_(pred, v1, v2, v3, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSUME_PRED3(pred, v1, v2, v3)		\
	IUTEST_PRED3_(pred, v1, v2, v3, IUTEST_ASSUME_FAILURE)

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSERT_PRED_FORMAT3(pred_formatter, v1, v2, v3)		\
	IUTEST_PRED_FORMAT3_(pred_formatter, v1, v2, v3, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_EXPECT_PRED_FORMAT3(pred_formatter, v1, v2, v3)		\
	IUTEST_PRED_FORMAT3_(pred_formatter, v1, v2, v3, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_INFORM_PRED_FORMAT3(pred_formatter, v1, v2, v3)		\
	IUTEST_PRED_FORMAT3_(pred_formatter, v1, v2, v3, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSUME_PRED_FORMAT3(pred_formatter, v1, v2, v3)		\
	IUTEST_PRED_FORMAT3_(pred_formatter, v1, v2, v3, IUTEST_ASSUME_FAILURE)

// 4
/** @private */
template<typename PRED, typename T1, typename T2, typename T3, typename T4>
AssertionResult	AssertPred4Helper(const char* pred_str, const char* expr1, const char* expr2, const char* expr3, const char* expr4
							  , PRED pred, T1 val1, T2 val2, T3 val3, T4 val4)
{
	if( (*pred)(val1, val2, val3, val4) ) return AssertionSuccess();
	return AssertionFailure() << "error: " << pred_str << "("
		<< expr1 << ", " << expr2 << ", " << expr3 << ", " << expr4
		<< ") evaluates to false, where "
		<< "\n" << expr1 << " : " << val1
		<< "\n" << expr2 << " : " << val2
		<< "\n" << expr3 << " : " << val3
		<< "\n" << expr4 << " : " << val4 ;
}

/**
 * @private
 * @{
*/
#define IUTEST_PRED4_(pred, v1, v2, v3, v4, on_failure)	\
	IUTEST_TEST_ASSERT_(::iutest::AssertPred4Helper(#pred, #v1, #v2, #v3, #v4, pred, v1, v2, v3, v4), on_failure)
#define IUTEST_PRED_FORMAT4_(pred_formatter, v1, v2, v3, v4, on_failure)	\
	IUTEST_TEST_ASSERT_(pred_formatter(#v1, #v2, #v3, #v4, v1, v2, v3, v4), on_failure)
/**
 * @}
*/

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSERT_PRED4(pred, v1, v2, v3, v4)		\
	IUTEST_PRED4_(pred, v1, v2, v3, v4, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_EXPECT_PRED4(pred, v1, v2, v3, v4)		\
	IUTEST_PRED4_(pred, v1, v2, v3, v4, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_INFORM_PRED4(pred, v1, v2, v3, v4)		\
	IUTEST_PRED4_(pred, v1, v2, v3, v4, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSUME_PRED4(pred, v1, v2, v3, v4)		\
	IUTEST_PRED4_(pred, v1, v2, v3, v4, IUTEST_ASSUME_FAILURE)

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSERT_PRED_FORMAT4(pred_formatter, v1, v2, v3, v4)		\
	IUTEST_PRED_FORMAT4_(pred_formatter, v1, v2, v3, v4, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_EXPECT_PRED_FORMAT4(pred_formatter, v1, v2, v3, v4)		\
	IUTEST_PRED_FORMAT4_(pred_formatter, v1, v2, v3, v4, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_INFORM_PRED_FORMAT4(pred_formatter, v1, v2, v3, v4)		\
	IUTEST_PRED_FORMAT4_(pred_formatter, v1, v2, v3, v4, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSUME_PRED_FORMAT4(pred_formatter, v1, v2, v3, v4)		\
	IUTEST_PRED_FORMAT4_(pred_formatter, v1, v2, v3, v4, IUTEST_ASSUME_FAILURE)


// 5
/** @private */
template<typename PRED, typename T1, typename T2, typename T3, typename T4, typename T5>
AssertionResult	AssertPred5Helper(const char* pred_str, const char* expr1, const char* expr2, const char* expr3, const char* expr4, const char* expr5
							  , PRED pred, T1 val1, T2 val2, T3 val3, T4 val4, T5 val5)
{
	if( (*pred)(val1, val2, val3, val4, val5) ) return AssertionSuccess();
	return AssertionFailure() << "error: " << pred_str << "("
		<< expr1 << ", " << expr2 << ", " << expr3 << ", " << expr4 << ", " << expr5
		<< ") evaluates to false, where "
		<< "\n" << expr1 << " : " << val1
		<< "\n" << expr2 << " : " << val2
		<< "\n" << expr3 << " : " << val3
		<< "\n" << expr4 << " : " << val4
		<< "\n" << expr5 << " : " << val5 ;
}

/**
 * @private
 * @{
*/
#define IUTEST_PRED5_(pred, v1, v2, v3, v4, v5, on_failure)	\
	IUTEST_TEST_ASSERT_(::iutest::AssertPred5Helper(#pred, #v1, #v2, #v3, #v4, #v5, pred, v1, v2, v3, v4, v5), on_failure)
#define IUTEST_PRED_FORMAT5_(pred_formatter, v1, v2, v3, v4, v5, on_failure)	\
	IUTEST_TEST_ASSERT_(pred_formatter(#v1, #v2, #v3, #v4, #v5, v1, v2, v3, v4, v5), on_failure)
/**
 * @}
*/

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSERT_PRED5(pred, v1, v2, v3, v4, v5)		\
	IUTEST_PRED5_(pred, v1, v2, v3, v4, v5, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_EXPECT_PRED5(pred, v1, v2, v3, v4, v5)		\
	IUTEST_PRED5_(pred, v1, v2, v3, v4, v5, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_INFORM_PRED5(pred, v1, v2, v3, v4, v5)		\
	IUTEST_PRED5_(pred, v1, v2, v3, v4, v5, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSUME_PRED5(pred, v1, v2, v3, v4, v5)		\
	IUTEST_PRED5_(pred, v1, v2, v3, v4, v5, IUTEST_ASSUME_FAILURE)

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSERT_PRED_FORMAT5(pred_formatter, v1, v2, v3, v4, v5)		\
	IUTEST_PRED_FORMAT5_(pred_formatter, v1, v2, v3, v4, v5, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_EXPECT_PRED_FORMAT5(pred_formatter, v1, v2, v3, v4, v5)		\
	IUTEST_PRED_FORMAT5_(pred_formatter, v1, v2, v3, v4, v5, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_INFORM_PRED_FORMAT5(pred_formatter, v1, v2, v3, v4, v5)		\
	IUTEST_PRED_FORMAT5_(pred_formatter, v1, v2, v3, v4, v5, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSUME_PRED_FORMAT5(pred_formatter, v1, v2, v3, v4, v5)		\
	IUTEST_PRED_FORMAT5_(pred_formatter, v1, v2, v3, v4, v5, IUTEST_ASSUME_FAILURE)


#if IUTEST_HAS_VARIADIC_TEMPLATES

/** @private */
template<typename PRED, typename ...Args>
AssertionResult	AssertPredVariadicHelper(const char* pred_str, const char* params, PRED pred, Args... args)
{
	if( (*pred)(args...) ) return AssertionSuccess();
	return AssertionFailure() << "error: " << pred_str << "("
		<< params << ") evaluates to false: "
		<< "\n" << PrintToStrings("\n", args...);
}
/**
 * @private
 * @{
*/
#define IUTEST_PRED_(pred, on_failure, ...)	\
	IUTEST_TEST_ASSERT_(::iutest::AssertPredVariadicHelper(#pred, #__VA_ARGS__, pred, __VA_ARGS__), on_failure)
/**
 * @}
*/

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSERT_PRED(pred, ...)		\
	IUTEST_PRED_(pred, IUTEST_ASSERT_FAILURE, __VA_ARGS__)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_EXPECT_PRED(pred, ...)		\
	IUTEST_PRED_(pred, IUTEST_EXPECT_FAILURE, __VA_ARGS__)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_INFORM_PRED(pred, ...)		\
	IUTEST_PRED_(pred, IUTEST_INFORM_FAILURE, __VA_ARGS__)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g
*/
#define IUTEST_ASSUME_PRED(pred, ...)		\
	IUTEST_PRED_(pred, IUTEST_ASSUME_FAILURE, __VA_ARGS__)

#endif

#ifndef IUTEST_NO_VARIADIC_MACROS

/**
 * @private
 * @{
*/
#define IUTEST_PRED_FORMAT_(pred_formatter, on_failure, ...)	\
	IUTEST_TEST_ASSERT_(pred_formatter(#__VA_ARGS__, __VA_ARGS__), on_failure)
/**
 * @}
*/

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSERT_PRED_FORMAT(pred_formatter, ...)		\
	IUTEST_PRED_FORMAT_(pred_formatter, IUTEST_ASSERT_FAILURE, __VA_ARGS__)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_EXPECT_PRED_FORMAT(pred_formatter, ...)		\
	IUTEST_PRED_FORMAT_(pred_formatter, IUTEST_EXPECT_FAILURE, __VA_ARGS__)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_INFORM_PRED_FORMAT(pred_formatter, ...)		\
	IUTEST_PRED_FORMAT_(pred_formatter, IUTEST_INFORM_FAILURE, __VA_ARGS__)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	�q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSUME_PRED_FORMAT(pred_formatter, ...)		\
	IUTEST_PRED_FORMAT_(pred_formatter, IUTEST_ASSUME_FAILURE, __VA_ARGS__)

#endif

/**
 * @private
 * @{
*/
#define IUTEST_THROW_PRED_FORMAT2_(pred_formatter, statement, expected_exception, expected_exception_value, on_failure)	\
	IUTEST_TEST_THROW_VALUE_(statement, expected_exception, expected_exception_value, on_failure, pred_formatter)
/**
 * @}
*/

/**
 * @ingroup	IUTEST_ASSERT_
 * @brief	��O�l �q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSERT_THROW_PRED_FORMAT2(pred_formatter, statement, expected_exception, expected_exception_value)		\
	IUTEST_THROW_PRED_FORMAT2_(pred_formatter, statement, expected_exception, expected_exception_value, IUTEST_ASSERT_FAILURE)
/**
 * @ingroup	IUTEST_EXPECT_
 * @brief	��O�l �q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_EXPECT_THROW_PRED_FORMAT2(pred_formatter, statement, expected_exception, expected_exception_value)		\
	IUTEST_THROW_PRED_FORMAT2_(pred_formatter, statement, expected_exception, expected_exception_value, IUTEST_EXPECT_FAILURE)
/**
 * @ingroup	IUTEST_INFORM_
 * @brief	��O�l �q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_INFORM_THROW_PRED_FORMAT2(pred_formatter, statement, expected_exception, expected_exception_value)		\
	IUTEST_THROW_PRED_FORMAT2_(pred_formatter, statement, expected_exception, expected_exception_value, IUTEST_INFORM_FAILURE)
/**
 * @ingroup	IUTEST_ASSUME_
 * @brief	��O�l �q�� ASSERTION �e�X�g(���b�Z�[�W�t�H�[�}�b�^�[�w��)
*/
#define IUTEST_ASSUME_THROW_PRED_FORMAT2(pred_formatter, statement, expected_exception, expected_exception_value)		\
	IUTEST_THROW_PRED_FORMAT2_(pred_formatter, statement, expected_exception, expected_exception_value, IUTEST_ASSUME_FAILURE)


}	// end of namespace iutest

#endif