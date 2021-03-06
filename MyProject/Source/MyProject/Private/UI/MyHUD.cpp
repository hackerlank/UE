#include "MyProject.h"
#include "UI/MyHUD.h"

#include "CanvasItem.h"
#include "MyDialog.h"
//#include "EngineGlobals.h"		// GEngine
#include "Engine.h"

#define LOCTEXT_NAMESPACE "MyProject.HUD"

AMyHUD::AMyHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UFont> BigFontOb(TEXT("/Game/UI/HUD/Roboto51"));
	BigFont = BigFontOb.Object;
	HUDDark = FColor(110, 124, 131, 255);
	AHUD* aa = dynamic_cast<AHUD*>(this);

	//static ConstructorHelpers::FClassFinder<UUserWidget> aaa(TEXT("/Game/Blueprints/NewWidgetBlueprint"));

	//if (aaa.Class != NULL)
	//{
	//	TSubclassOf<class UUserWidget> WidgetClass = aaa.Class;
	//	UUserWidget* PawnToFit = WidgetClass->GetDefaultObject<UUserWidget>();

		//NewWidget = Cast<UUserWidget>(StaticConstructObject(*WidgetClass));
		//NewWidget->AddToViewport();
	//}
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
	if (Canvas == nullptr)
	{
		return;
	}

	FString Text = LOCTEXT("WaitingForRespawn", "WAITING FOR RESPAWN").ToString();
	FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), BigFont, HUDDark);
	TextItem.EnableShadow(FLinearColor::Black);
	TextItem.Text = FText::FromString(Text);
	TextItem.Scale = FVector2D(1.0f, 1.0f);
	//TextItem.FontRenderInfo = ShadowedFont;
	TextItem.SetColor(FLinearColor(0.75f, 0.125f, 0.125f, 1.0f));
	Canvas->DrawItem(TextItem);

	if (!DialogWidget.IsValid())
	{
		// warning C4996: 'SMyDialog::FArguments::MessageText': Passing text to Slate as FString is deprecated, please use FText instead (likely via a LOCTEXT). Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
		//DialogWidget = SNew(SMyDialog)
		//	.MessageText("asdfasdasdfadsf");

		DialogWidget = SNew(SMyDialog)
			.MessageText(LOCTEXT("MyHUB", "asdfasdasdfadsf"));

		SMyDialog::FArguments aaa;
		//aaa.MessageText("asdfasdasdfadsf");
		aaa.MessageText(LOCTEXT("MyHUB", "asdfasdasdfadsf"));
		DialogWidget->Construct(aaa);

		//GEngine->GameViewport->AddViewportWidgetContent(DialogWidget.ToSharedRef());

		//NewWidget->AddToViewport();

		//TSharedPtr<SWidget> OutUserSlateWidget;
		//TSharedRef<SWidget> RootWidget = NewWidget->MakeViewportWidget(OutUserSlateWidget);
		//GEngine->GameViewport->AddViewportWidgetContent(RootWidget);
		//UWidget* bbb = NewWidget->GetContentForSlot("aaa");
		
		//TSharedPtr<SButton> bbb = MakeShareable((SButton*)(NewWidget->GetSlateWidgetFromName("Button_16").Get()));

		//UButton* ccc = (UButton*)NewWidget->GetWidgetFromName("Button_14");
		//FWeakObjectPtr fff(this);
		//TScriptDelegate<FWeakObjectPtr> ddd;
		//TScriptDelegate<AMyHUD> ddd;

		//ddd.BindUFunction((UObject *)this, "OnConfirmGeneric");
		//ccc->OnClicked.Add(ddd);

		//bbb->SetOnClicked(FOnClicked::CreateRaw(this, &AMyHUD::OnConfirmGeneric));
		//bbb->SetOnClicked(FOnClicked::CreateSP(this, &AMyHUD::OnConfirmGeneric));
		//bbb->SetOnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, OnConfirmGeneric));

		//FOnClicked InOnClicked = FOnClicked::CreateUObject(this, &AMyHUD::OnConfirmGeneric);
		//bbb->SetOnClicked(InOnClicked);
	}

	//NewWidget->AddToViewport();
}

//FReply AMyHUD::OnConfirmGeneric()
void AMyHUD::OnConfirmGeneric()
{
	//return FReply::Handled();
}