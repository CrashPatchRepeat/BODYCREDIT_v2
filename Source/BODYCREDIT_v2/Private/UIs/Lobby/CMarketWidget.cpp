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

	APlayerController* lpc=UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ACNoxController* PC=Cast<ACNoxController>(lpc);
	const ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(PC->GetPawn());
	InventoryComp = PlayerCharacter->GetComponentByClass<UCInventoryComponent>();

	InventoryGridWidget->InitInventory(InventoryComp, InventoryComp->GetInventoryTileSize());
	InventoryGridWidget->SetGridId(0);
	InventoryGridWidget->SetPC(PC);

	MarketComp = PlayerCharacter->GetComponentByClass<UCMarketComponent>();

	OnSelectWeaponClicked();

	GS = GetWorld()->GetGameState<ACGameState>();
	GI = Cast<UCGameInstance>(GetGameInstance());
	
	if (GI) {
		GI->MarketUI = this;
		GI->OnGoldChanged.Clear();
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
	DisplayMarketItems(EPlayerPart::Weapon1, Btn_SelectWeapon);
}

void UCMarketWidget::OnSelectHeadClicked()
{
	// 머리 버튼 클릭 시 처리
	DisplayMarketItems(EPlayerPart::Head, Btn_SelectHead);
}


void UCMarketWidget::OnSelectBodyClicked()
{
	// 몸통 버튼 클릭 시 처리
	DisplayMarketItems(EPlayerPart::Body, Btn_SelectBody);
}

void UCMarketWidget::OnSelectArmClicked()
{
	// 팔 버튼 클릭 시 처리
	DisplayMarketItems(EPlayerPart::Arm, Btn_SelectArm);
}

void UCMarketWidget::OnSelectLegClicked()
{
	// 다리 버튼 클릭 시 처리
	DisplayMarketItems(EPlayerPart::Leg, Btn_SelectLeg);
}

void UCMarketWidget::OnSelectBackpackClicked()
{
	// 백팩 버튼 클릭 시 처리
	DisplayMarketItems(EPlayerPart::Backpack, Btn_SelectBackpack);
}

void UCMarketWidget::UpdatePlayerGoldText(const int32 NewGold)
{
	if (Txt_PlayerGold) Txt_PlayerGold->SetText(FText::AsNumber(NewGold));
}

void UCMarketWidget::DisplayMarketItems(EPlayerPart ItemType, UCLobbyButtonBase* PressButton)
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
		return;
	
	if (PreviousButton == PressButton) return;
	PreviousButton = PressButton;

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Items = MarketComp->GetMarketItems(ItemType);

	const int32 NumPerRow = 2;
	TArray<UHorizontalBox*> RowBoxes;

	const int32 NumRows = FMath::CeilToInt(static_cast<float>(Items.Num()) / NumPerRow);
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		UHorizontalBox* NewRow = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		RowBoxes.Add(NewRow);
	}

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		int32 RowIndex = i / NumPerRow;

		UCMarketTileWidget* ItemWidget = CreateWidget<UCMarketTileWidget>(Cast<ACNoxController>(GetOwningPlayerPawn()->GetController()), MarketItemWidget);
		if (ItemWidget)
		{
			ItemWidget->OwningMarket = this;
			if (Items[i].Thumbnail)
			{
				FSlateBrush NewBrush= ItemWidget->GetItemBrush();
				NewBrush.SetResourceObject(Items[i].Thumbnail);
				ItemWidget->SetItemImage(NewBrush, Items[i], InventoryComp);
			}

			UHorizontalBox* TargetRow = RowBoxes[RowIndex];
			if (TargetRow)
			{
				UHorizontalBoxSlot* SlotH = TargetRow->AddChildToHorizontalBox(ItemWidget);
				if (SlotH)
				{
					SlotH->SetHorizontalAlignment(HAlign_Center);
					SlotH->SetVerticalAlignment(VAlign_Center);
					SlotH->SetPadding(FMargin(20,0));
				}
			}
		}
	}

	for (UHorizontalBox* RowBox : RowBoxes)
	{
		if (RowBox)
		{
			UVerticalBoxSlot* VSlot = VerticalBox_MarketItem->AddChildToVerticalBox(RowBox);
			if (VSlot) VSlot->SetHorizontalAlignment(HAlign_Fill);
		}
	}
}


void UCMarketWidget::RemoveWidget()
{
}
