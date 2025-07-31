// Copyright by wcyiming


#include "AbilitySystem/Data/AttributeInfo.h"

FAuroraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const {
	for (const FAuroraAttributeInfo& Info : AttributeInformation) {
		if (Info.AttributeTag.MatchesTagExact(AttributeTag)) {
			return Info;
		}
	}

	if (bLogNotFound) {
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}


	return FAuroraAttributeInfo();
}
