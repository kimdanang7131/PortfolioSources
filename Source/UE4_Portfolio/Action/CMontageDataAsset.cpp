#include "Action/CMontageDataAsset.h"

TArray<FMontageData> UCMontageDataAsset::GetMontageDatas() const
{
	return MontageDatas;
}

FMontageData UCMontageDataAsset::GetMontageData(const int& idx)
{
	if (idx >= MontageDatas.Num())
		return FMontageData();

	return MontageDatas[idx];
}

