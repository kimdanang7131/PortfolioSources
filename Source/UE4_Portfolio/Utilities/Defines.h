#pragma once

#define NULL_RETURN(p){ if(p == NULL) return ; }
#define NULL_RETURN_RESULT(p, result){ if(p == NULL) return result;}

#define TRUE_RETURN(p){ if(p == true) return ; }
#define TRUE_RETURN_RESULT(p, result){ if(p == true) return result; }

#define FALSE_RETURN(p){ if(p == false) return ; }
#define FALSE_RETURN_RESULT(p, result){ if(p == false) return result; }

//template <typename T>
//FORCEINLINE void ReturnIfNull(T p)
//{
//	if (p == NULL)
//		return;
//}
//
//template <typename T1 ,typename T2>
//FORCEINLINE T2 ReturnResultIfNull(T1 p , T2 result)
//{
//	if (p == NULL)
//		return result;
//}
//
//template <typename T>
//FORCEINLINE void CheckTrueReturn(T p)
//{
//	if (p == true)
//		return;
//}
//
//template <typename T>
//FORCEINLINE void ReturnFalse(T p)
//{
//	if (p == NULL)
//		return;
//}
//
//template <typename T>
//FORCEINLINE void ReturnFalse(T p)
//{
//	if (p == NULL)
//		return;
//}
//
//template <typename T>
//FORCEINLINE void ReturnFalse(T p)
//{
//	if (p == NULL)
//		return;
//}
//
//template <typename T>
//FORCEINLINE void ReturnFalse(T p)
//{
//	if (p == NULL)
//		return;
//}