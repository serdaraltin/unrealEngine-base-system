// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CineCameraComponent.h"
#include "CineCameraActor.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ComboBoxString.h"

#include "MainUserWidget.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class LockingAxis : uint8
{
	X     UMETA(DisplayName = "X Axis"),
	Y     UMETA(DisplayName = "Y Axis"),
	Z     UMETA(DisplayName = "Z Axis"),
	Roll  UMETA(DisplayName = "Roll Axis"),
	Pitch UMETA(DisplayName = "Pitch Axis"),
	Yaw   UMETA(DisplayName = "Yaw Axis")
};


UCLASS()
class CAMERA_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
		void SetUp(class AMyActor* InMyActor);
	UFUNCTION()
		void Teardown();
	UFUNCTION()
		void SaveSession(const FString& SlotName, const int32 UserIndex);
	UFUNCTION()
		void HideInterfaceMenu();

	UFUNCTION()
		void OnFilmBackChanged(FString Item,
			ESelectInfo::Type SelectionType);

	UFUNCTION()
		void LoadFilmBackPresets(uint32 FilmBackIndex);

	UFUNCTION()
		void OnMultiCamChanged(FString Item,
			ESelectInfo::Type SelectionType);

	UFUNCTION()
		void LoadMultiCamBox();

	UFUNCTION()
		void OnFocusSettingsButtonCliked();

	UFUNCTION()
		void OnAxisLockingButtonCliked();

	UFUNCTION()
		void OnFocalLengthButtonCliked();

	UFUNCTION()
		void OnCameraSettingsButtonCliked();

	UFUNCTION()
		void OnMultiCamButtonCliked();

	UFUNCTION()
		void OnPlusButtonCliked();
	UFUNCTION()
		void OnMinusButtonCliked();

	UFUNCTION()
		void OnStabilizationMultiButtonCliked();

	UFUNCTION()
		void OnCheckboxStabilizationChanged(bool Value);

	UFUNCTION()
		void OnShowDrawDebugLine(bool Show);

	UFUNCTION()
		void OnStabilizationValueChanged(float Value);

	UFUNCTION()
		void OnSmoothLensValueChanged(float Value);

	UFUNCTION()
		void OnPhoneMultiplicationValueChanged(float Value);

	UFUNCTION()
		void LeftMultiplicationValueChanged(float Value);

	UFUNCTION()
		void RightMultiplicationValueChanged(float Value);

	UFUNCTION()
		void FocalLengthValueChanged(float Value);

	UFUNCTION()
		void ApertureValueChanged(float Value);

	UPROPERTY(BlueprintReadWrite, Category = " MainUserWidg")
		bool isJoystickOn;

	UPROPERTY(BlueprintReadWrite, Category = " MainUserWidg")
		bool isRecordingVideo;

	UPROPERTY(meta = (BindWidget))
		class UWidget* InterfaceMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MovementSubMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* AxisLockingSubMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* FocalLengthWidget;

	UPROPERTY(meta = (BindWidget))
		class UWidget* FocalLengthBackground;

	UPROPERTY(meta = (BindWidget))
		class UButton* HideInterfaceButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* StabilizationMultiButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* AxisLockingButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* FocalLengthButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* StabilizationValueText;

	UPROPERTY(meta = (BindWidget))
		class UButton* CameraSettingsButton;

	UPROPERTY(meta = (BindWidget))
		class UWidget* CameraSettingsSubMenu;

	UPROPERTY(meta = (BindWidget))
		class UComboBoxString* FilmBackComboBox;

	UPROPERTY(meta = (BindWidget))
		class USlider* SmoothLensSlider;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SmoothLensText;

	//===========Multi Camera sub menu ================================================================

	UPROPERTY(meta = (BindWidget))
		class UButton* MultiCamButton;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MultiCamSubMenu;

	UPROPERTY(meta = (BindWidget))
		class UComboBoxString* MultiCamComboBox;

	UPROPERTY(meta = (BindWidget))
		class UButton* AxesMultiCamButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MultiCamTextBlock;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* WarningTextBlock;

	UFUNCTION()
		void OnAxesMultiCamButtonCliked();

	UFUNCTION()
		void FinishCameraChanged();

	//===========Freeze phone tracking sub menu ================================================================

	UPROPERTY(meta = (BindWidget))
		class UButton* FreezeMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UWidget* FreezeSubMenu;

	UFUNCTION()
		void OnFreezeMenuButtonCliked();

	UPROPERTY(meta = (BindWidget))
		class UButton* StopTrackingButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ContinueTrackingButton;

	UFUNCTION()
		void OnStopTrackingButtonCliked();

	UFUNCTION()
		void OnContinueTrackingButtonCliked();

	//===========Focus Menu Settings ================================================================

	UPROPERTY(meta = (BindWidget))
		class UButton* FocusSettingsButton;

	UPROPERTY(meta = (BindWidget))
		class UWidget* SubFocSettingsMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* DebugTextBlock;

	UPROPERTY(meta = (BindWidget))
		class UWidget* FocusSetBackground;

	UPROPERTY(meta = (BindWidget))
		class UCheckBox* DebugPlaneCheckBox;

	UPROPERTY(meta = (BindWidget))
		class USlider* ApertureSlider;

	UPROPERTY(meta = (BindWidget))
		class UButton* PlusFocDistanceButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MinusFocDistanceButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* FocusDistanceTextBlock;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* FocusDistanceTextReplication;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ApertureTextReplication;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ApertureTextBlockOne;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock*  ApertureTextBlockTwo;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock*  ApertureTextBlockThree;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock*  ApertureTextBlockFour;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock*  ApertureTextBlockFive;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock*  ApertureTextBlockSix;

	//===========Lens Text blocks binding ================================================================
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LensTextBlockOne;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LensTextBlockTwo;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LensTextBlockThree;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LensTextBlockFour;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LensTextBlockFive;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LensTextBlockSix;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LensTextBlockSeven;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LensTextReplication;
	//===========Lens Text blocks END ================================================================

	UPROPERTY(meta = (BindWidget))
		class USlider* StabilizationSlider;

	UPROPERTY(meta = (BindWidget))
		class USlider* PhoneMultiplicatorSlider;

	UPROPERTY(meta = (BindWidget))
		class USlider* LeftMultiplicatorSlider;

	UPROPERTY(meta = (BindWidget))
		class USlider* RightMultiplicatorSlider;

	UPROPERTY(meta = (BindWidget))
		class USlider* FocalLenghtSlider;

	UPROPERTY(meta = (BindWidget))
		class UCheckBox* StabilizationCheckBox;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* SubMenuWidgetSwitcher;

	UPROPERTY(BlueprintReadWrite, Category = " MainUserWidg")
		ACameraActor* TargetCamera;

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		void ActivateGameViewport();

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		void EnableAutoFocus();

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		void DisableAutoFocus();

	UFUNCTION(BlueprintPure, Category = " MainUserWidg")
		UButton* GetPreviousSettingsButton();

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		bool GetAutoFocusStatus();//if true -> AutoFocusEnabled, false -> Manual

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		float GetStabilizerStrength();

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		float GetPhoneMultiplicationValue();

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		float GetLeftMultiplicationValue();

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		float GetRightMultiplicationValue();

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		float GetSmoothLensValue();

	//Update ALL interface!!!!!!
	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		void UploadInterfaceData(const FString& SlotName, const int32 UserIndex);

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		void OnAxisLockButtonPressed(UButton* PressedButton, LockingAxis AxisToLock);

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		void LoadAxisLockButton(UButton* Button, LockingAxis Axis);

	UFUNCTION(BlueprintCallable, Category = " MainUserWidg")
		void OpenFocusSettingsMenu(bool Open);

	UFUNCTION(BlueprintImplementableEvent, Category = " MainUserWidg")
		void OnCameraChanged(ACineCameraActor* NewCamera, ACineCameraActor* PreviousCamera);

	void SetMyActorPtr(class AMyActor* InMyActor);

	void EnableStabilization(bool Is_Enabled);

	void SubMenuWidgetChange(UWidget* WidgetToSet);

	void SubMenuWidgetChange(int WidgetNumber);

	float SmoothLensValue;

	FString LastCameraName;


protected:

	UPROPERTY(BlueprintReadWrite, Category = " MainUserWidg")
	class AMyActor* MyActor;

	bool Is_MenuHidden;

	bool Is_FocusSettingsClosed;

	bool is_focalLengthMenuOpened;

	virtual bool Initialize() override;

	float focusDistanceMeters;

	float ApertureSliderValue;
	float LensSliderValue;

	float LensValueMilim;
private:

	FRotator DeltaTracking;

	UButton* PreviousSettingsButton;

};

