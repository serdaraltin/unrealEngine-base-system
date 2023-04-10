// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUserWidget.h"
#include "MyActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "MySaveGame.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"

#include "Framework/Application/SlateApplication.h"


void UMainUserWidget::SetUp(class AMyActor* InMyActor)
{
	MyActor = InMyActor;
	TargetCamera = MyActor->CineCamPtr;

	this->AddToViewport();

	LastCameraName = "MainCamera";

	UploadInterfaceData(LastCameraName, 0);
	OpenFocusSettingsMenu(false);
	LoadMultiCamBox();
	SubMenuWidgetSwitcher->SetActiveWidgetIndex(0);
	AxesMultiCamButton->SetIsEnabled(false);
}

void UMainUserWidget::Teardown()
{

	SaveSession(LastCameraName, 0);

	this->RemoveFromViewport();

}

void UMainUserWidget::SaveSession(const FString& SlotName, const int32 UserIndex)
{

	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	//SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("SavedInterfaceSettings", 1));

	SaveGameInstance->FocusDistance = focusDistanceMeters;
	SaveGameInstance->ApertureValue = ApertureSliderValue;
	SaveGameInstance->LensValue = LensSliderValue;
	SaveGameInstance->SmoothLensValue = SmoothLensValue;
	SaveGameInstance->FilmbackPresetIndex = FilmBackComboBox->GetSelectedIndex();
	SaveGameInstance->Is_AutoFocusEnabled = MyActor->AutoFocus;
	SaveGameInstance->StabilizationValue = MyActor->StabilizerStrength;
	SaveGameInstance->PhoneMultiplicationValue = MyActor->MultiplicationByNumber;
	SaveGameInstance->LeftMultiplicationValue = MyActor->LeftJoystickMultiplication;
	SaveGameInstance->RightMultiplicationValue = MyActor->RightJoystickMultiplication;
	SaveGameInstance->ActiveWidgetIndex = SubMenuWidgetSwitcher->GetActiveWidgetIndex();
	SaveGameInstance->LensValueMilimiters = LensValueMilim;

	if (ECheckBoxState::Checked == DebugPlaneCheckBox->GetCheckedState())
		SaveGameInstance->Is_DebugPlaneEnabled = true;
	else
		SaveGameInstance->Is_DebugPlaneEnabled = false;

	if (ECheckBoxState::Checked == StabilizationCheckBox->GetCheckedState())
		SaveGameInstance->Is_StabilizationEnabled = true;
	else
		SaveGameInstance->Is_StabilizationEnabled = false;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, UserIndex);
}

void UMainUserWidget::ActivateGameViewport()
{
	FSlateApplication::Get().ActivateGameViewport();
}

void UMainUserWidget::EnableAutoFocus()
{
	if (!ensure(MyActor != nullptr))
		return;

	MyActor->EnableAutoFocus();

	DebugPlaneCheckBox->SetIsChecked(false);

}

void UMainUserWidget::DisableAutoFocus()
{
	if (!ensure(MyActor != nullptr))
		return;

	MyActor->DisableAutoFocus();

	MyActor->CineCamPtr->GetCineCameraComponent()->FocusSettings.ManualFocusDistance = (focusDistanceMeters * 100);
	MyActor->CineCamPtr->GetCineCameraComponent()->FocusSettings.bSmoothFocusChanges = false;
}

UButton* UMainUserWidget::GetPreviousSettingsButton()
{
	return PreviousSettingsButton;
}

bool UMainUserWidget::GetAutoFocusStatus()
{


	if (MyActor != nullptr) {

		return MyActor->AutoFocus;
	}

	return true;
}

float UMainUserWidget::GetStabilizerStrength()
{

	return StabilizationSlider->GetValue();

}

float UMainUserWidget::GetPhoneMultiplicationValue()
{

	return PhoneMultiplicatorSlider->GetValue();
}

float UMainUserWidget::GetLeftMultiplicationValue()
{
	return  LeftMultiplicatorSlider->GetValue();
}

float UMainUserWidget::GetRightMultiplicationValue()
{
	return RightMultiplicatorSlider->GetValue();
}

float UMainUserWidget::GetSmoothLensValue()
{
	return SmoothLensSlider->GetValue();
}

void UMainUserWidget::UploadInterfaceData(const FString& SlotName, const int32 UserIndex)
{
	if (MyActor != nullptr) {

		if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex)) {
			UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, UserIndex);
		}

		//UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
		MyActor->StabilizerStrength = SaveGameInstance->StabilizationValue;
		MyActor->MultiplicationByNumber = SaveGameInstance->PhoneMultiplicationValue;
		MyActor->LeftJoystickMultiplication = SaveGameInstance->LeftMultiplicationValue;
		MyActor->RightJoystickMultiplication = SaveGameInstance->RightMultiplicationValue;
		MyActor->EnableStabilizing = SaveGameInstance->Is_StabilizationEnabled;
		MyActor->AutoFocus = SaveGameInstance->Is_AutoFocusEnabled;
		ApertureSlider->SetValue(SaveGameInstance->ApertureValue);
		ApertureValueChanged(SaveGameInstance->ApertureValue);

		StabilizationSlider->SetValue(MyActor->StabilizerStrength);
		OnStabilizationValueChanged(MyActor->StabilizerStrength);

		OnShowDrawDebugLine(SaveGameInstance->Is_DebugPlaneEnabled);
		DebugPlaneCheckBox->SetIsChecked(SaveGameInstance->Is_DebugPlaneEnabled);

		PhoneMultiplicatorSlider->SetValue(MyActor->Stabilizer(0.f, 3000.f, 0.f, 3.f, MyActor->MultiplicationByNumber));
		OnPhoneMultiplicationValueChanged(MyActor->Stabilizer(0.f, 3000.f, 0.f, 3.f, MyActor->MultiplicationByNumber));
		LeftMultiplicatorSlider->SetValue(MyActor->Stabilizer(0.f, 3000.f, 0.f, 3.f, MyActor->LeftJoystickMultiplication));
		LeftMultiplicationValueChanged(MyActor->Stabilizer(0.f, 3000.f, 0.f, 3.f, MyActor->LeftJoystickMultiplication));
		RightMultiplicatorSlider->SetValue(MyActor->Stabilizer(0.f, 3000.f, 0.f, 3.f, MyActor->RightJoystickMultiplication));
		RightMultiplicationValueChanged(MyActor->Stabilizer(0.f, 3000.f, 0.f, 3.f, MyActor->RightJoystickMultiplication));
		EnableStabilization(MyActor->EnableStabilizing);
		//SubMenuWidgetSwitcher->SetActiveWidgetIndex(0);
		FocalLenghtSlider->Locked = true;
		FocalLengthWidget->SetVisibility(ESlateVisibility::Hidden);
		FocalLengthBackground->SetVisibility(ESlateVisibility::Hidden);
		focusDistanceMeters = SaveGameInstance->FocusDistance;
		FString str(FString::FromInt((focusDistanceMeters)));
		str.Append(FString("m"));
		FocusDistanceTextBlock->SetText(FText::FromString(str));
		FocusDistanceTextReplication->SetText(FText::FromString(str));
		SmoothLensValue = SaveGameInstance->SmoothLensValue;
		SmoothLensSlider->SetValue(SmoothLensValue);
		OnSmoothLensValueChanged(SmoothLensValue);
		LoadFilmBackPresets(SaveGameInstance->FilmbackPresetIndex);
		
		MyActor->TargetLensValue = SaveGameInstance->LensValueMilimiters;
		MyActor->CineCamPtr->GetCineCameraComponent()->CurrentFocalLength = SaveGameInstance->LensValueMilimiters;
		FocalLenghtSlider->SetValue(SaveGameInstance->LensValue);
		FocalLengthValueChanged(SaveGameInstance->LensValue);

		//LoadMultiCamBox();

		if (!SaveGameInstance->Is_AutoFocusEnabled) {
			MyActor->CineCamPtr->GetCineCameraComponent()->FocusSettings.ManualFocusDistance = focusDistanceMeters * 100;
		}

		ContinueTrackingButton->SetIsEnabled(false);
	}

}

void UMainUserWidget::OnAxisLockButtonPressed(UButton * PressedButton, LockingAxis AxisToLock)
{
	if (!ensure(MyActor != nullptr))
		return;
	bool is_locked = false;
	switch (AxisToLock)
	{
	case LockingAxis::X:

		MyActor->AxisMovement_X = !(MyActor->AxisMovement_X);
		if (!MyActor->AxisMovement_X)
			is_locked = true;
		break;

	case LockingAxis::Y:

		MyActor->AxisMovement_Y = !(MyActor->AxisMovement_Y);
		if (!MyActor->AxisMovement_Y)
			is_locked = true;
		break;

	case LockingAxis::Z:

		MyActor->AxisMovement_Z = !(MyActor->AxisMovement_Z);
		if (!MyActor->AxisMovement_Z)
			is_locked = true;
		break;

	case LockingAxis::Roll:

		MyActor->AxisRotation_Roll = !(MyActor->AxisRotation_Roll);
		MyActor->NewActorRotation.Roll = MyActor->GetActorRotation().Roll;
		if (!MyActor->AxisRotation_Roll)
			is_locked = true;
		break;

	case LockingAxis::Pitch:

		MyActor->AxisRotation_Pitch = !(MyActor->AxisRotation_Pitch);
		MyActor->NewActorRotation.Pitch = MyActor->GetActorRotation().Pitch;
		if (!MyActor->AxisRotation_Pitch)
			is_locked = true;
		break;

	case LockingAxis::Yaw:

		MyActor->AxisRotation_Yaw = !(MyActor->AxisRotation_Yaw);
		MyActor->NewActorRotation.Yaw = MyActor->GetActorRotation().Yaw;
		if (!MyActor->AxisRotation_Yaw)
			is_locked = true;
		break;

	default:
		break;
	}

	if (is_locked) {
		PressedButton->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		PressedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
	}
	else {
		PressedButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		PressedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UMainUserWidget::LoadAxisLockButton(UButton * Button, LockingAxis Axis)
{
	bool is_locked = false;

	switch (Axis)
	{
	case LockingAxis::X:

		is_locked = MyActor->AxisMovement_X;

		break;

	case LockingAxis::Y:

		is_locked = MyActor->AxisMovement_Y;
		break;

	case LockingAxis::Z:

		is_locked = MyActor->AxisMovement_Z;
		break;

	case LockingAxis::Roll:

		is_locked = MyActor->AxisRotation_Roll;
		break;

	case LockingAxis::Pitch:

		is_locked = MyActor->AxisRotation_Pitch;

		break;

	case LockingAxis::Yaw:

		is_locked = MyActor->AxisRotation_Yaw;

		break;

	default:
		break;
	}

	if (!is_locked) {
		Button->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		Button->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
	}
	else {
		Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Button->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UMainUserWidget::SetMyActorPtr(AMyActor* InMyActor)
{
	MyActor = InMyActor;
}

void UMainUserWidget::EnableStabilization(bool Is_Enabled)
{
	if (Is_Enabled) {
		StabilizationSlider->SetIsEnabled(true);
		StabilizationSlider->SetValue(MyActor->StabilizerStrength);
		StabilizationCheckBox->SetIsChecked(true);
		MyActor->EnableStabilizing = true;
	}
	else {
		StabilizationSlider->SetIsEnabled(false);
		StabilizationSlider->SetValue(0.f);
		StabilizationCheckBox->SetIsChecked(false);
		MyActor->EnableStabilizing = false;
	}

}

void UMainUserWidget::SubMenuWidgetChange(UWidget* WidgetToSet)
{

	SubMenuWidgetSwitcher->SetActiveWidget(WidgetToSet);
}

void UMainUserWidget::SubMenuWidgetChange(int WidgetNumber)
{
	SubMenuWidgetSwitcher->SetActiveWidgetIndex(WidgetNumber);
}

void UMainUserWidget::HideInterfaceMenu() {

	Is_MenuHidden = !Is_MenuHidden;

	if (Is_MenuHidden)
		InterfaceMenu->SetVisibility(ESlateVisibility::Hidden);
	else
		InterfaceMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}




void UMainUserWidget::OnFocusSettingsButtonCliked()
{
	//Is_FocusSettingsClosed = !Is_FocusSettingsClosed;
	OpenFocusSettingsMenu(Is_FocusSettingsClosed);

}

void UMainUserWidget::OpenFocusSettingsMenu(bool Open)
{
	if (Open) {
		ApertureSlider->SetLocked(false);
		SubFocSettingsMenu->SetVisibility(ESlateVisibility::Visible);
		DebugTextBlock->SetVisibility(ESlateVisibility::Visible);
		FocusSetBackground->SetVisibility(ESlateVisibility::Visible);
		DebugPlaneCheckBox->SetVisibility(ESlateVisibility::Visible);
		Is_FocusSettingsClosed = false;

		FocusSettingsButton->SetRenderTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1), FVector2D(0, 0), 180));

	}
	else {
		ApertureSlider->SetLocked(true);
		SubFocSettingsMenu->SetVisibility(ESlateVisibility::Hidden);
		DebugTextBlock->SetVisibility(ESlateVisibility::Hidden);
		FocusSetBackground->SetVisibility(ESlateVisibility::Hidden);
		DebugPlaneCheckBox->SetVisibility(ESlateVisibility::Hidden);
		Is_FocusSettingsClosed = true;
		FocusSettingsButton->SetRenderTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1), FVector2D(0, 0), 0));

	}

}

void UMainUserWidget::OnCameraSettingsButtonCliked()
{

	PreviousSettingsButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	CameraSettingsButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.2f));
	PreviousSettingsButton = CameraSettingsButton;
	SubMenuWidgetChange(CameraSettingsSubMenu);
}

void UMainUserWidget::OnMultiCamButtonCliked()
{
	PreviousSettingsButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	MultiCamButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.2f));
	PreviousSettingsButton = MultiCamButton;
	SubMenuWidgetChange(MultiCamSubMenu);

}

void UMainUserWidget::OnAxisLockingButtonCliked()
{
	PreviousSettingsButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	AxisLockingButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.2f));
	PreviousSettingsButton = AxisLockingButton;
	SubMenuWidgetChange(AxisLockingSubMenu);
}

void UMainUserWidget::OnFocalLengthButtonCliked()
{
	is_focalLengthMenuOpened = !is_focalLengthMenuOpened;
	if (is_focalLengthMenuOpened) {
		FocalLenghtSlider->SetLocked(false);
		FocalLengthWidget->SetVisibility(ESlateVisibility::Visible);
		FocalLengthBackground->SetVisibility(ESlateVisibility::Visible);
		FocalLengthButton->SetRenderTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1), FVector2D(0, 0), 180));

	}
	else {
		FocalLenghtSlider->SetLocked(true);
		FocalLengthWidget->SetVisibility(ESlateVisibility::Hidden);
		FocalLengthBackground->SetVisibility(ESlateVisibility::Hidden);
		FocalLengthButton->SetRenderTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1), FVector2D(0, 0), 0));
	}
}


void UMainUserWidget::OnPlusButtonCliked()
{

	if (focusDistanceMeters == 0) {
		MyActor->CineCamPtr->GetCineCameraComponent()->FocusSettings.FocusMethod = ECameraFocusMethod::Manual;
		if (DebugPlaneCheckBox->GetCheckedState() == ECheckBoxState::Checked)
			OnShowDrawDebugLine(true);

		focusDistanceMeters += 1;
	}
	else {
		focusDistanceMeters += 2;
	}

	focusDistanceMeters = FMath::Clamp(focusDistanceMeters, 0.f, 100.f);
	MyActor->CineCamPtr->GetCineCameraComponent()->FocusSettings.ManualFocusDistance = (focusDistanceMeters * 100);
	FString str(FString::FromInt((focusDistanceMeters)));
	str.Append(FString("m"));
	FocusDistanceTextBlock->SetText(FText::FromString(str));
	FocusDistanceTextReplication->SetText(FText::FromString(str));
}

void UMainUserWidget::OnMinusButtonCliked()
{
	focusDistanceMeters -= 2;
	focusDistanceMeters = FMath::Clamp(focusDistanceMeters, 0.f, 100.f);
	if (focusDistanceMeters == 0) {
		MyActor->CineCamPtr->GetCineCameraComponent()->FocusSettings.FocusMethod = ECameraFocusMethod::Disable;
		OnShowDrawDebugLine(false);
		FocusDistanceTextBlock->SetText(FText::FromString(FString("Off")));
		FocusDistanceTextReplication->SetText(FText::FromString(FString("Off")));
	}
	else {
		MyActor->CineCamPtr->GetCineCameraComponent()->FocusSettings.ManualFocusDistance = (focusDistanceMeters * 100);
		FString str(FString::FromInt((focusDistanceMeters)));
		str.Append(FString("m"));
		FocusDistanceTextBlock->SetText(FText::FromString(str));
		FocusDistanceTextReplication->SetText(FText::FromString(str));
	}
}

void UMainUserWidget::OnStabilizationMultiButtonCliked()
{
	PreviousSettingsButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	StabilizationMultiButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.2f));
	PreviousSettingsButton = StabilizationMultiButton;
	SubMenuWidgetChange(MovementSubMenu);
}

void UMainUserWidget::OnAxesMultiCamButtonCliked()
{

	OnContinueTrackingButtonCliked();

	AxesMultiCamButton->SetIsEnabled(false);
	MultiCamTextBlock->SetText(FText::FromString("Axes are unlocked"));
	WarningTextBlock->SetText(FText::FromString(""));

	MyActor->EnableStabilizing = true;

}

void UMainUserWidget::OnFreezeMenuButtonCliked()
{
	PreviousSettingsButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	FreezeMenuButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.2f));
	PreviousSettingsButton = FreezeMenuButton;
	SubMenuWidgetChange(FreezeSubMenu);

}

void UMainUserWidget::OnStopTrackingButtonCliked()
{

	MyActor->AxisMovement_X = false;
	MyActor->AxisMovement_Y = false;
	MyActor->AxisMovement_Z = false;
	MyActor->AxisRotation_Roll = false;
	MyActor->AxisRotation_Pitch = false;
	MyActor->AxisRotation_Yaw = false;
	MyActor->NewActorRotation = MyActor->GetActorRotation();
	//DeltaTracking = MyActor->PhoneRotation;
	MyActor->DeltaStopTracking = FRotator(0, 0, 0);

	ContinueTrackingButton->SetIsEnabled(true);
	StopTrackingButton->SetIsEnabled(false);
}

void UMainUserWidget::OnContinueTrackingButtonCliked()
{

	MyActor->DeltaStopTracking = MyActor->PhoneRotation;

	MyActor->ZeroLocationCoordinate = MyActor->GetActorLocation();

	MyActor->StartActorRotation.Yaw = MyActor->GetActorRotation().Yaw - MyActor->DeltaStopTracking.Yaw;
	//MyActor->StartActorRotation.Yaw = MyActor->GetActorRotation().Yaw ;
	MyActor->AxisMovement_X = true;
	MyActor->AxisMovement_Y = true;
	MyActor->AxisMovement_Z = true;
	MyActor->AxisRotation_Roll = true;
	MyActor->AxisRotation_Pitch = true;
	MyActor->AxisRotation_Yaw = true;

	ContinueTrackingButton->SetIsEnabled(false);
	StopTrackingButton->SetIsEnabled(true);
}



void UMainUserWidget::OnCheckboxStabilizationChanged(bool Value)
{
	EnableStabilization(Value);
}

void UMainUserWidget::OnShowDrawDebugLine(bool Show)
{
	if (Show) {
		MyActor->CineCamPtr->GetCineCameraComponent()->FocusSettings.bDrawDebugFocusPlane = true;
	}
	else {
		MyActor->CineCamPtr->GetCineCameraComponent()->FocusSettings.bDrawDebugFocusPlane = false;
	}
}

void UMainUserWidget::OnStabilizationValueChanged(float Value)
{
	Value = FMath::Clamp(Value, 0.f, 1.f);
	MyActor->StabilizerStrength = Value;

	//StabilizationValueText->SetText(FText(FText::AsNumber(Value)));
}

void UMainUserWidget::OnSmoothLensValueChanged(float Value)
{

	SmoothLensValue = Value;

}

void UMainUserWidget::OnPhoneMultiplicationValueChanged(float Value)
{
	if (!ensure(MyActor != nullptr))
		return;

	MyActor->MultiplicationByNumber = MyActor->Stabilizer(0.f, 3.f, 0.f, 3000.f, Value);

}

void UMainUserWidget::LeftMultiplicationValueChanged(float Value)
{
	if (!ensure(MyActor != nullptr))
		return;

	MyActor->LeftJoystickMultiplication = MyActor->Stabilizer(0.f, 3.f, 0.f, 3000.f, Value);
}

void UMainUserWidget::RightMultiplicationValueChanged(float Value)
{
	if (!ensure(MyActor != nullptr))
		return;

	MyActor->RightJoystickMultiplication = MyActor->Stabilizer(0.f, 3.f, 0.f, 3000.f, Value);
}

void UMainUserWidget::FocalLengthValueChanged(float Value)
{
	LensSliderValue = Value;
	FString LensArray[18] =
	{
		TEXT("  "),   TEXT("  "),  TEXT("  "),TEXT("185mm"),TEXT("125mm"),TEXT("100mm"),
		TEXT("80mm"), TEXT("65mm"),TEXT("50mm"),TEXT("40mm"),
		TEXT("35mm"),TEXT("32mm"),TEXT("27mm"),
		TEXT("24mm"), TEXT("21mm"),
		TEXT("  "),   TEXT("  "),  TEXT("  ")
	};

	UTextBlock* BlockArray[7] = { LensTextBlockOne, LensTextBlockTwo, LensTextBlockThree,
			LensTextBlockFour, LensTextBlockFive , LensTextBlockSix , LensTextBlockSeven
	};

	int lensNum = Value;
	for (int i = 0; i < 7; i++, lensNum++) {
		lensNum = FMath::Clamp(lensNum, 0, 17);
		BlockArray[i]->SetText(FText::FromString(LensArray[lensNum]));

	}

	LensTextReplication->SetText(LensTextBlockFour->GetText());

	float LensValue = 35.f;
	switch (((int(Value)) - 3))
	{
	case -3:
		LensValue = 185.f;
		break;
	case -2:
		LensValue = 125.f;
		break;
	case -1:
		LensValue = 100.f;
		break;
	case 0:
		LensValue = 80.f;
		break;
	case 1:
		LensValue = 65.f;
		break;
	case 2:
		LensValue = 50.f;
		break;
	case 3:
		LensValue = 40.f;
		break;
	case 4:
		LensValue = 35.f;
		break;
	case 5:
		LensValue = 32.f;
		break;
	case 6:
		LensValue = 27.f;
		break;
	case 7:
		LensValue = 24.f;
		break;
	case 8:
		LensValue = 21.f;
		break;
	default:
		break;
	}

	MyActor->TargetLensValue = LensValue;
	LensValueMilim = LensValue;
}

void UMainUserWidget::ApertureValueChanged(float Value)
{
	ApertureSliderValue = Value;
	FString LensArray[12] =
	{
		TEXT("  "),   TEXT("  "),  TEXT("  "),
		TEXT("f/8"), TEXT("f/5.6"),TEXT("f/4"),
		TEXT("f/2.8"),TEXT("f/2"),TEXT("f/1.4"),
		TEXT("  "),   TEXT("  "),  TEXT("  ")
	};

	UTextBlock* BlockArray[6] = { ApertureTextBlockOne, ApertureTextBlockTwo, ApertureTextBlockThree,
			ApertureTextBlockFour, ApertureTextBlockFive , ApertureTextBlockSix
	};

	int lensNum = Value;
	for (int i = 0; i < 6; i++, lensNum++) {
		lensNum = FMath::Clamp(lensNum, 0, 11);
		BlockArray[i]->SetText(FText::FromString(LensArray[lensNum]));

	}

	ApertureTextReplication->SetText(ApertureTextBlockFour->GetText());

	float ApertureValue = 2.8f;
	switch (int(Value))
	{
	case 0:
		ApertureValue = 8.f;
		break;
	case 1:
		ApertureValue = 5.6f;
		break;
	case 2:
		ApertureValue = 4.f;
		break;
	case 3:
		ApertureValue = 2.8f;
		break;
	case 4:
		ApertureValue = 2.f;
		break;
	case 5:
		ApertureValue = 1.4f;
		break;
	default:
		break;
	}

	MyActor->CineCamPtr->GetCineCameraComponent()->CurrentAperture = ApertureValue;
}

void UMainUserWidget::OnFilmBackChanged(FString Item, ESelectInfo::Type SelectionType)
{
	//UE_LOG(LogTemp, Warning, TEXT("Changed"));

	TArray<FNamedFilmbackPreset> const& Presets = UCineCameraComponent::GetFilmbackPresets();
	int32 const NumPresets = Presets.Num();
	for (int32 PresetIdx = 0; PresetIdx < NumPresets; ++PresetIdx)
	{
		FNamedFilmbackPreset const& P = Presets[PresetIdx];
		if (P.Name == Item)
		{
			MyActor->CineCamPtr->GetCineCameraComponent()->Filmback = P.FilmbackSettings;
			break;
		}
	}
}

void UMainUserWidget::LoadFilmBackPresets(uint32 FilmBackIndex)
{

	TArray<FNamedFilmbackPreset> const& Presets = UCineCameraComponent::GetFilmbackPresets();
	int32 const NumPresets = Presets.Num();
	if (FilmBackComboBox->GetOptionCount() > 1) {
		FilmBackComboBox->ClearOptions();
	}
	for (int32 PresetIdx = 0; PresetIdx < NumPresets; ++PresetIdx)
	{
		
		FNamedFilmbackPreset const& P = Presets[PresetIdx];

		FilmBackComboBox->AddOption(P.Name);
		if (PresetIdx == FilmBackIndex) {
			FilmBackComboBox->SetSelectedIndex(FilmBackIndex);
			MyActor->CineCamPtr->GetCineCameraComponent()->Filmback = P.FilmbackSettings;
		}
	}

}

void UMainUserWidget::OnMultiCamChanged(FString Item, ESelectInfo::Type SelectionType)
{
	SaveSession(LastCameraName, 0);

	ACineCameraActor* PreviousCamera = MyActor->CineCamPtr;
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	OurPlayerController->PlayerCameraManager->StartCameraFade(0, 1, 0.6f, FLinearColor::Black, false, true);
	FTimerHandle handle;
	MyActor->GetWorldTimerManager().SetTimer(handle, this, &UMainUserWidget::FinishCameraChanged, 0.6f);


	if (Item == "MainCamera") {

		MyActor->DeltaStopTracking = MyActor->PhoneRotation;
		MyActor->ZeroLocationCoordinate = MyActor->CineCamPtrCopy->GetActorLocation();
		//MyActor->StartActorRotation.Yaw = MyActor->CineCamPtrCopy->GetActorRotation().Yaw - MyActor->DeltaStopTracking.Yaw;
		MyActor->StartActorRotation = MyActor->CineCamPtrCopy->GetActorRotation();
		MyActor->DeltaStopTracking = FRotator(0, 0, 0);

		MyActor->MainCamera = MyActor->CineCamPtrCopy;
		MyActor->CineCamPtr = MyActor->CineCamPtrCopy;
		MyActor->SetActorLocation(MyActor->CineCamPtrCopy->GetActorLocation());
		MyActor->SetActorRotation(MyActor->CineCamPtrCopy->GetActorRotation());
		MyActor->MainCamera->SetActorLocation(MyActor->CineCamPtrCopy->GetActorLocation());
		MyActor->MainCamera->SetActorRotation(MyActor->CineCamPtrCopy->GetActorRotation());
		MyActor->CurrentSampleV = MyActor->GetActorLocation();
		MyActor->CurrentSampleR = MyActor->GetActorRotation();
		MyActor->NewActorRotation = MyActor->GetActorRotation();

		//OurPlayerController->SetViewTargetWithBlend(MyActor->MainCamera, 0.0f);
	}
	else {
		TArray<ACineCameraActor*> const CineCamArr = MyActor->MultiCineCamera;
		int32 const NumPresets = CineCamArr.Num();
		for (int32 PresetIdx = 0; PresetIdx < NumPresets; ++PresetIdx)
		{
			if (CineCamArr[PresetIdx] == nullptr)
				continue;

			if (CineCamArr[PresetIdx]->GetName() == Item) {


				MyActor->DeltaStopTracking = MyActor->PhoneRotation;
				MyActor->ZeroLocationCoordinate = CineCamArr[PresetIdx]->GetActorLocation();
				//MyActor->StartActorRotation.Yaw = CineCamArr[PresetIdx]->GetActorRotation().Yaw - MyActor->DeltaStopTracking.Yaw;
				MyActor->StartActorRotation = CineCamArr[PresetIdx]->GetActorRotation();
				MyActor->DeltaStopTracking = FRotator(0, 0, 0);

				MyActor->MainCamera = CineCamArr[PresetIdx];
				MyActor->CineCamPtr = CineCamArr[PresetIdx];
				MyActor->SetActorLocation(CineCamArr[PresetIdx]->GetActorLocation());
				MyActor->SetActorRotation(CineCamArr[PresetIdx]->GetActorRotation());
				MyActor->CurrentSampleV = MyActor->GetActorLocation();
				MyActor->CurrentSampleR = MyActor->GetActorRotation();
				MyActor->NewActorRotation = MyActor->GetActorRotation();


				//OurPlayerController->SetViewTargetWithBlend(CineCamArr[PresetIdx], 0.0f);

				//MyActor->StartActorRotation.Yaw = MyActor->GetActorRotation().Yaw ;

			}
		}
	}

	OnCameraChanged(MyActor->CineCamPtr, PreviousCamera);



	LastCameraName = Item;
	UploadInterfaceData(LastCameraName, 0);

	MyActor->EnableStabilizing = false;


	if (MyActor->StartActorRotation.Yaw >= 0)
		MyActor->positive = true;
	else
		MyActor->positive = false;

	AxesMultiCamButton->SetIsEnabled(true);
	MultiCamTextBlock->SetText(FText::FromString("Unlock All Axes"));
	WarningTextBlock->SetText(FText::FromString("Axes are locked! Press to unlock:"));


	OnStopTrackingButtonCliked();


}

void UMainUserWidget::FinishCameraChanged()
{
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	OurPlayerController->SetViewTargetWithBlend(MyActor->MainCamera, 0.0f);

	OurPlayerController->PlayerCameraManager->StartCameraFade(1, 0, 1, FLinearColor::Black);

}

void UMainUserWidget::LoadMultiCamBox()
{

	TArray<ACineCameraActor*> const CineCamArr = MyActor->MultiCineCamera;
	int32 const NumPresets = CineCamArr.Num();
	//MultiCamComboBox->AddOption("MainCamera");
	//MultiCamComboBox->SetSelectedIndex(0);
	for (int32 PresetIdx = 0; PresetIdx < NumPresets; ++PresetIdx)
	{
		if (CineCamArr[PresetIdx] == nullptr)
			continue;

		FString const& P = CineCamArr[PresetIdx]->GetName();

		MultiCamComboBox->AddOption(P);

	}
	MyActor->EnableStabilizing = true;
	MyActor->AxisMovement_X = true;
	MyActor->AxisMovement_Y = true;
	MyActor->AxisMovement_Z = true;
	MyActor->AxisRotation_Roll = true;
	MyActor->AxisRotation_Pitch = true;
	MyActor->AxisRotation_Yaw = true;

}


bool UMainUserWidget::Initialize()
{
	bool is_succeed = Super::Initialize();
	if (!is_succeed)
		return false;

	Is_MenuHidden = false;
	isJoystickOn = false;
	isRecordingVideo = false;
	is_focalLengthMenuOpened = false;
	Is_FocusSettingsClosed = false;
	MyActor = nullptr;
	ApertureSliderValue = 3;
	LensSliderValue = 7;

	HideInterfaceButton->OnClicked.AddDynamic(this, &UMainUserWidget::HideInterfaceMenu);
	StabilizationSlider->OnValueChanged.AddDynamic(this, &UMainUserWidget::OnStabilizationValueChanged);

	PhoneMultiplicatorSlider->OnValueChanged.AddDynamic(this, &UMainUserWidget::OnPhoneMultiplicationValueChanged);
	LeftMultiplicatorSlider->OnValueChanged.AddDynamic(this, &UMainUserWidget::LeftMultiplicationValueChanged);
	RightMultiplicatorSlider->OnValueChanged.AddDynamic(this, &UMainUserWidget::RightMultiplicationValueChanged);
	StabilizationCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainUserWidget::OnCheckboxStabilizationChanged);

	StabilizationMultiButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnStabilizationMultiButtonCliked);
	AxisLockingButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnAxisLockingButtonCliked);

	PreviousSettingsButton = StabilizationMultiButton;

	FocalLengthButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnFocalLengthButtonCliked);
	FocalLenghtSlider->OnValueChanged.AddDynamic(this, &UMainUserWidget::FocalLengthValueChanged);

	FocusSettingsButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnFocusSettingsButtonCliked);
	DebugPlaneCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainUserWidget::OnShowDrawDebugLine);

	PlusFocDistanceButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnPlusButtonCliked);
	MinusFocDistanceButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnMinusButtonCliked);
	ApertureSlider->OnValueChanged.AddDynamic(this, &UMainUserWidget::ApertureValueChanged);

	CameraSettingsButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnCameraSettingsButtonCliked);
	FilmBackComboBox->OnSelectionChanged.AddDynamic(this, &UMainUserWidget::OnFilmBackChanged);

	SmoothLensSlider->OnValueChanged.AddDynamic(this, &UMainUserWidget::OnSmoothLensValueChanged);

	FreezeMenuButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnFreezeMenuButtonCliked);
	StopTrackingButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnStopTrackingButtonCliked);
	ContinueTrackingButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnContinueTrackingButtonCliked);

	MultiCamComboBox->OnSelectionChanged.AddDynamic(this, &UMainUserWidget::OnMultiCamChanged);
	MultiCamButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnMultiCamButtonCliked);
	AxesMultiCamButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnAxesMultiCamButtonCliked);

	return true;
}