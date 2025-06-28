#include "UIs/Lobby/CMarketWidget.h"

#include "UIs/Common/Buttons/CLobbyButtonBase.h"
#include "Components/Inventory/CInventoryComponent.h"
#include "Global.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Games/CNoxController.h"
#include "UIs/Inventory/CInventoryGrid.h"
#include "Components/Market/CMarketComponent.h"
#include "Games/CGameInstance.h"
#include "Games/CGameState.h"
#include "UIs/Market/CMarketTileWidget.h"

void UCMarketWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto lpc=UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto PC=Cast<ACNoxController>(lpc);
	ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(PC->GetPawn());
	InventoryComp = PlayerCharacter->GetComponentByClass<UCInventoryComponent>();

	InventoryGridWidget->InitInventory(InventoryComp, InventoryComp->GetInventoryTileSize());
	InventoryGridWidget->SetGridId(0);
	InventoryGridWidget->SetPC(PC);

	MarketComp = PlayerCharacter->GetComponentByClass<UCMarketComponent>();

	OnSelectWeaponClicked();
	PreviousImage = Image_SelectWeapon_Hovered;
	PreviousImage->SetVisibility(ESlateVisibility::Visible);

	GS = GetWorld()->GetGameState<ACGameState>();

	GI = Cast<UCGameInstance>(GetGameInstance());
	if (GI) {
		GI->MarketUI = this;
		GI->OnGoldChanged.RemoveDynamic(this, &UCMarketWidget::UpdatePlayerGoldText);
		GI->OnGoldChanged.AddDynamic(this, &UCMarketWidget::UpdatePlayerGoldText);
		GI->SetPlayerGold(GI->PlayerGold);

		// TODO 어차피 팝업으로 띄우는데 지울 필요가 있을까?싶음
		// GI->OnBack.RemoveAll(this);
		// GI->OnBack.AddUObject(this, &UCMarketWidget::RemoveWidget);
	}
}

void UCMarketWidget::OnSelectWeaponClicked()
{
	// 무기 버튼 클릭 시 처리
	CLog::Print("Select Weapon Clicked");
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}
	if (PreviousImage == Image_SelectWeapon_Hovered) return;
	TurnOffPreviousImage();
	PreviousImage = Image_SelectWeapon_Hovered;
	TurnOnPreviousImage();
	

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Weapons = MarketComp->GetMarketItems(EPlayerPart::Weapon1);

	const int32 NumPerRow = 2;
	TArray<UHorizontalBox*> RowBoxes;

	const int32 NumRows = FMath::CeilToInt((float)Weapons.Num() / NumPerRow);
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		UHorizontalBox* NewRow = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		RowBoxes.Add(NewRow);
	}

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		int32 RowIndex = i / NumPerRow;

		UCMarketTileWidget* ItemWidget = CreateWidget<UCMarketTileWidget>(this, MarketItemWidget);
		if (ItemWidget)
		{
			ItemWidget->OwningMarket = this;
			if (Weapons[i].Thumbnail)
			{
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(Weapons[i].Thumbnail);
				NewBrush.ImageSize = ImageSize;
				ItemWidget->SetItemImage(NewBrush, Weapons[i], InventoryComp);
			}

			UHorizontalBox* TargetRow = RowBoxes[RowIndex];
			if (TargetRow)
			{
				UHorizontalBoxSlot* SlotH = TargetRow->AddChildToHorizontalBox(ItemWidget);
				if (Slot)
				{
					SlotH->SetHorizontalAlignment(HAlign_Center);
					SlotH->SetVerticalAlignment(VAlign_Center);
				}
			}
		}
	}

	for (UHorizontalBox* RowBox : RowBoxes)
	{
		if (RowBox)
		{
			UVerticalBoxSlot* VSlot = VerticalBox_MarketItem->AddChildToVerticalBox(RowBox);
			if (VSlot)
			{
				VSlot->SetHorizontalAlignment(HAlign_Fill);
			}
		}
	}
}

void UCMarketWidget::OnSelectHeadClicked()
{
	// 머리 버튼 클릭 시 처리
	CLog::Print("Select Head Clicked");
}

void UCMarketWidget::OnSelectBodyClicked()
{
	// 몸통 버튼 클릭 시 처리
	CLog::Print("Select Body Clicked");
}

void UCMarketWidget::OnSelectArmClicked()
{
	// 팔 버튼 클릭 시 처리
	CLog::Print("Select Arm Clicked");
}

void UCMarketWidget::OnSelectLegClicked()
{
	// 다리 버튼 클릭 시 처리
	CLog::Print("Select Leg Clicked");
}

void UCMarketWidget::OnSelectBackpackClicked()
{
	// 백팩 버튼 클릭 시 처리
	CLog::Print("Select Backpack Clicked");
}

void UCMarketWidget::UpdatePlayerGoldText(int32 NewGold)
{
	if (Txt_PlayerGold)
	{
		Txt_PlayerGold->SetText(FText::AsNumber(NewGold));
	}
}

void UCMarketWidget::RemoveWidget()
{
}

void UCMarketWidget::TurnOnPreviousImage()
{
	if (PreviousImage)
		PreviousImage->SetVisibility(ESlateVisibility::Visible);
}

void UCMarketWidget::TurnOffPreviousImage()
{
	if (PreviousImage)
		PreviousImage->SetVisibility(ESlateVisibility::Hidden);
}
