#include "Action/CMontageDataAsset.h"

/** Montage 자체를 얻기 */
TArray<FMontageData> UCMontageDataAsset::GetMontageDatas() const
{
	return MontageDatas;
}

/** Montage에서 특정 idx의 Montage 얻기 */
FMontageData UCMontageDataAsset::GetMontageData(const int& idx)
{
	if (idx >= MontageDatas.Num())
		return FMontageData();

	return MontageDatas[idx];
}

