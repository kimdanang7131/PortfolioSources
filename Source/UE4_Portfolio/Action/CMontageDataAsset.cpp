#include "Action/CMontageDataAsset.h"

/** Montage ��ü�� ��� */
TArray<FMontageData> UCMontageDataAsset::GetMontageDatas() const
{
	return MontageDatas;
}

/** Montage���� Ư�� idx�� Montage ��� */
FMontageData UCMontageDataAsset::GetMontageData(const int& idx)
{
	if (idx >= MontageDatas.Num())
		return FMontageData();

	return MontageDatas[idx];
}

