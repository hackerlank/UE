#pragma once

#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class SMyDialog;

UCLASS()
class AMyHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:
	/** Main HUD update loop. */
	virtual void DrawHUD() override;

public:
	//UFUNCTION(BlueprintCallable, Category = "Button")
	//FReply OnConfirmGeneric();

	UFUNCTION(BlueprintCallable, Category = "Button")
	void OnConfirmGeneric();

protected:
	/** kills background icon. */
	UPROPERTY()
	FCanvasIcon KillsBg;

	/** Large font - used for ammo display etc. */
	UPROPERTY()
	UFont* BigFont;

	/** Darker HUD color. */
	FColor HUDDark;

	TSharedPtr<SMyDialog>			DialogWidget;
	//UUserWidget* NewWidget;
};
