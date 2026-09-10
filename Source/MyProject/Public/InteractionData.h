#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InteractionData.generated.h"

USTRUCT(BlueprintType)
struct FInteractionData
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText InteractionText = FText::GetEmpty();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText DisplayName = FText::GetEmpty();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TObjectPtr<const UInputAction> InputAction = nullptr;
};