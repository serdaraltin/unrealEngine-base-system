// Copyright 2017-2021 David Romanski(Socke). All Rights Reserved.

#include "JSONParserBPLibrary.h"

UJSONParserBPLibrary* UJSONParserBPLibrary::JSONParserBPLibrary;
UJSONParserBPLibrary::UJSONParserBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	JSONParserBPLibrary = this;
}



UJSONParserBPLibrary* UJSONParserBPLibrary::getJSONParserBPLibrary() {
	return JSONParserBPLibrary;
}

//create tmap
TMap<FString, FString> UJSONParserBPLibrary::MakeMapString(FString Key, FString Value) {

	TMap<FString, FString> map;
	map.Add(Key, Value);
	return map;
}

TMap<FString, float> UJSONParserBPLibrary::MakeMapNumber(FString Key, float Value) {

	TMap<FString, float> map;
	map.Add(Key, Value);
	return map;
}

TMap<FString, bool> UJSONParserBPLibrary::MakeMapBool(FString Key, bool Value) {

	TMap<FString, bool> map;
	map.Add(Key, Value);
	return map;
}

TMap<FString, FSJsonObject> UJSONParserBPLibrary::MakeMapObject(FString Key, FSJsonObject Value) {

	TMap<FString, FSJsonObject> map;
	map.Add(Key, Value);
	return map;
}

//add elements to tmap
TMap<FString, FString> UJSONParserBPLibrary::AddMapString(TMap<FString, FString> map, FString Key, FString Value) {
	map.Add(Key, Value);
	return map;
}

TMap<FString, float> UJSONParserBPLibrary::AddMapNumber(TMap<FString, float> map, FString Key, float Value) {
	map.Add(Key, Value);
	return map;
}

TMap<FString, bool> UJSONParserBPLibrary::AddMapBool(TMap<FString, bool> map, FString Key, bool Value) {
	map.Add(Key, Value);
	return map;
}

TMap<FString, FSJsonObject> UJSONParserBPLibrary::AddMapObject(TMap<FString, FSJsonObject> map, FString Key, FSJsonObject Value) {
	map.Add(Key, Value);
	return map;
}

//change values in tmap
TMap<FString, FString> UJSONParserBPLibrary::ChangeMapString(TMap<FString, FString> map, FString Key, FString Value) {
	map.Remove(Key);
	map.Add(Key, Value);
	return map;
}

TMap<FString, float> UJSONParserBPLibrary::ChangeMapNumber(TMap<FString, float> map, FString Key, float Value) {

	map.Remove(Key);
	map.Add(Key, Value);
	return map;
}

TMap<FString, bool> UJSONParserBPLibrary::ChangeMapBool(TMap<FString, bool> map, FString Key, bool Value) {

	map.Remove(Key);
	map.Add(Key, Value);
	return map;
}

TMap<FString, FSJsonObject> UJSONParserBPLibrary::ChangeMapObject(TMap<FString, FSJsonObject> map, FString Key, FSJsonObject Value) {

	map.Remove(Key);
	map.Add(Key, Value);
	return map;
}

//json stuff
FString UJSONParserBPLibrary::MakeString(FString Value) {
	return Value;
}

float UJSONParserBPLibrary::MakeNumber(float Value) {
	return Value;
}

bool UJSONParserBPLibrary::MakeBool(bool Value) {
	return Value;
}



//void UJSONParserBPLibrary::findStringInJsonObject(FSJsonObject jsonObject, FString key, bool searchInArrayObjects, bool printLogs, FString& value){
//	
//	if (jsonObject.ue4Object == nullptr) {
//		UE_LOG(LogTemp, Warning, TEXT("JsonObject is Empty"));
//		return;
//	}
//
//
//	findStringInternalInJsonObject(jsonObject.ue4Object, key, searchInArrayObjects,printLogs, value);
//}
//
//void UJSONParserBPLibrary::findStringInternalInJsonObject(TSharedPtr<FJsonObject> jsonObject, FString key, bool searchInArrayObjects, bool printLogs, FString& value){
//
//
//	if (jsonObject == nullptr) {
//		return;
//	}
//
//	if (jsonObject->TryGetStringField(key, value)) {
//		if (printLogs)
//			UE_LOG(LogTemp, Warning, TEXT("%s found: %s"), *key, *value);
//		return;
//	}
//
//
//	for (auto& element : jsonObject->Values) {
//		if (element.Value->Type == EJson::Object) {
//			if (printLogs)
//				UE_LOG(LogTemp, Warning, TEXT("Search in Object: %s"), *element.Key);
//			findStringInternalInJsonObject(element.Value->AsObject(), key,searchInArrayObjects,printLogs,value);
//			if (value.IsEmpty() == false) {
//				return;
//			}
//		}
//		else {
//			if (searchInArrayObjects && element.Value->Type == EJson::Array) {
//				TArray<TSharedPtr<FJsonValue>> arr = element.Value->AsArray();
//				for (int32 i = 0; i < arr.Num(); i++){
//					if (arr[i] != nullptr && arr[i]->Type == EJson::Object) {
//						if (printLogs)
//							UE_LOG(LogTemp, Warning, TEXT("Search in ObjectArray[%i]: %s"), i, *element.Key);
//						findStringInternalInJsonObject(arr[i]->AsObject(), key, searchInArrayObjects,printLogs, value);
//						if (value.IsEmpty() == false) {
//							return;
//						}
//					}
//				}
//			}
//		}
//
//	}
//}





struct FSJsonObject UJSONParserBPLibrary::MakeJson_Object(TMap<FString, FSJsonObject> ObjectMap, TMap<FString, bool> BoolMap, TMap<FString, float> NumberMap, TMap<FString, FString> StringMap, TArray<FSJsonObject> ObjectArray, TArray<bool> BoolArray, TArray<float> NumberArray, TArray<FString> StringArray) {
	FSJsonObject mainObject = generateFSJsonObjectWithParameter(ObjectMap, BoolMap, NumberMap, StringMap, ObjectArray, BoolArray, NumberArray, StringArray);
	return mainObject;
}





struct FSJsonObject UJSONParserBPLibrary::generateFSJsonObjectWithParameter(TMap<FString, FSJsonObject> ObjectMap, TMap<FString, bool> BoolMap, TMap<FString, float> NumberMap, TMap<FString, FString> StringMap, TArray<FSJsonObject> ObjectArray, TArray<bool> BoolArray, TArray<float> NumberArray, TArray<FString> StringArray) {

	FSJsonObject mainObject;
	mainObject.ue4Object = MakeShareable(new FJsonObject());

	//maps
	for (auto& element : ObjectMap) {
		if (element.Value.ue4Object.IsValid()) {
			mainObject.ue4Object->SetObjectField(element.Key, element.Value.ue4Object);
			if (element.Value.ue4Array.Num() > 0) {
				mainObject.ue4Object->SetArrayField(element.Key, element.Value.ue4Array);
			}
		}
		else {
			//array as value like {"answers": ["asd","zxc","qwe","qaz","wsx"]}
			if (element.Value.asStringArray.Num() > 0) {
				TArray< TSharedPtr<FJsonValue>> a;
				for (int32 i = 0; i < element.Value.asStringArray.Num(); i++) {
					a.Add(MakeShareable(new FJsonValueString(element.Value.asStringArray[i])));
				}
				mainObject.ue4Object->SetArrayField(element.Key, a);
			}

			if (element.Value.asObjectArray.Num() > 0) {
				TArray< TSharedPtr<FJsonValue>> a;
				for (int32 i = 0; i < element.Value.asObjectArray.Num(); i++) {
					a.Add(MakeShareable(new FJsonValueObject(element.Value.asObjectArray[i].ue4Object)));
				}
				mainObject.ue4Object->SetArrayField(element.Key, a);
			}

			if (element.Value.asBoolArray.Num() > 0) {
				TArray< TSharedPtr<FJsonValue>> a;
				for (int32 i = 0; i < element.Value.asBoolArray.Num(); i++) {
					a.Add(MakeShareable(new FJsonValueBoolean(element.Value.asBoolArray[i])));
				}
				mainObject.ue4Object->SetArrayField(element.Key, a);
			}

			if (element.Value.asNumberArray.Num() > 0) {
				TArray< TSharedPtr<FJsonValue>> a;
				for (int32 i = 0; i < element.Value.asNumberArray.Num(); i++) {
					a.Add(MakeShareable(new FJsonValueNumber(element.Value.asNumberArray[i])));
				}
				mainObject.ue4Object->SetArrayField(element.Key, a);
			}
		}
	}
	for (auto& element : StringMap) {
		mainObject.ue4Object->SetStringField(element.Key, element.Value);
	}
	for (auto& element : BoolMap) {
		mainObject.ue4Object->SetBoolField(element.Key, element.Value);
	}
	for (auto& element : NumberMap) {
		mainObject.ue4Object->SetNumberField(element.Key, element.Value);
	}


	//arrays
	mainObject.asObjectArray = ObjectArray;
	mainObject.asBoolArray = BoolArray;
	mainObject.asNumberArray = NumberArray;
	mainObject.asStringArray = StringArray;


	if (mainObject.asObjectArray.Num() > 0) {
		for (int32 i = 0; i < mainObject.asObjectArray.Num(); i++) {
			if (mainObject.asObjectArray[i].ue4Object.IsValid())
				mainObject.ue4Array.Add(MakeShareable(new FJsonValueObject(mainObject.asObjectArray[i].ue4Object)));
		}
	}

	if (mainObject.asStringArray.Num() > 0) {
		for (int32 i = 0; i < mainObject.asStringArray.Num(); i++) {
			mainObject.ue4Array.Add(MakeShareable(new FJsonValueString(mainObject.asStringArray[i])));
		}
	}
	if (mainObject.asBoolArray.Num() > 0) {
		for (int32 i = 0; i < mainObject.asBoolArray.Num(); i++) {
			mainObject.ue4Array.Add(MakeShareable(new FJsonValueBoolean(mainObject.asBoolArray[i])));
		}
	}
	if (mainObject.asNumberArray.Num() > 0) {
		for (int32 i = 0; i < mainObject.asNumberArray.Num(); i++) {
			mainObject.ue4Array.Add(MakeShareable(new FJsonValueNumber(mainObject.asNumberArray[i])));
		}
	}

	return mainObject;
}

struct FSJsonObject UJSONParserBPLibrary::generateFSJsonObject(struct FSJsonObject object) {
	TMap<FString, FSJsonObject> ObjectMap;
	TMap<FString, bool> BoolMap;
	TMap<FString, float> NumberMap;
	TMap<FString, FString> StringMap;
	FSJsonObject mainObject = generateFSJsonObjectWithParameter(ObjectMap, BoolMap, NumberMap, StringMap, object.asObjectArray, object.asBoolArray, object.asNumberArray, object.asStringArray);
	return mainObject;
}


FString UJSONParserBPLibrary::JsonObjectToString(bool& successful, struct FSJsonObject mainObject, bool removeLineBreaks, bool addPadding) {

	FString data = "";
	TSharedRef<TJsonWriter<>> json_writer = TJsonWriterFactory<>::Create(&data);

	if (!mainObject.ue4Object.IsValid()) {
		mainObject = generateFSJsonObject(mainObject);
	}
	if (mainObject.ue4Object->Values.Num() == 0 && mainObject.ue4Array.Num() > 0) {
		 successful = FJsonSerializer::Serialize(mainObject.ue4Array, json_writer);
	}
	else {
		successful = FJsonSerializer::Serialize(mainObject.ue4Object.ToSharedRef(), json_writer);
	}

	if (!successful) {
		return data;
	}

	if (addPadding) {
		data = mainObject.jsonPBefore + data + mainObject.jsonPAfter;
	}

	if (removeLineBreaks) {

		//UE_LOG(LogTemp, Warning, TEXT("1:%s"), *data);
		data = data.Replace(TEXT("\r"), TEXT(""));
		data = data.Replace(TEXT("\n"), TEXT(""));
		data = data.Replace(TEXT("\t"), TEXT(""));
		//UE_LOG(LogTemp, Warning, TEXT("2:%s"), *data);
	}


	return data;

}





FSJsonObject UJSONParserBPLibrary::JsonStringToObject(FString data, bool isJsonP, bool& successful) {

	FSJsonObject mainObject;

	if (isJsonP) {
		if (data.Contains("{")) {
			if (data.Split("{", &mainObject.jsonPBefore, &data)) {
				data = "{" + data;
			}
		}
		if (data.Contains("}")) {
			int32 index = 0;
			if (data.FindLastChar(*"}", index)) {
				if ((index + 1) < data.Len())
					mainObject.jsonPAfter = data.RightChop(index + 1);
				data = data.Left(index);
				data += "}";
			}
		}
	}

	//UE_LOG(LogTemp, Display, TEXT("Before:%s"), *mainObject.jsonPBefore);
	//UE_LOG(LogTemp, Display, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"));
	//UE_LOG(LogTemp, Display, TEXT("After:%s"), *mainObject.jsonPAfter);
	//UE_LOG(LogTemp, Display, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"));
	//UE_LOG(LogTemp, Display, TEXT("%s"), *data);

	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(data);
	TArray< TSharedPtr<FJsonValue> > ue4ObjectArray;
	successful = FJsonSerializer::Deserialize(reader, ue4ObjectArray);
	if (successful) {

		for (int32 i = 0; i < ue4ObjectArray.Num(); i++) {
			//FMixedArrayObject mo;
			if (ue4ObjectArray[i]->Type == EJson::Object) {
				FSJsonObject o;
				o.ue4Object = ue4ObjectArray[i]->AsObject();
				mainObject.asObjectArray.Add(o);
				continue;
			}

			if (ue4ObjectArray[i]->Type == EJson::String) {
				mainObject.asStringArray.Add(ue4ObjectArray[i]->AsString());
				//mo.asString = ue4ObjectArray[i]->AsString();
				continue;
			}
			if (ue4ObjectArray[i]->Type == EJson::Boolean) {
				mainObject.asBoolArray.Add(ue4ObjectArray[i]->AsBool());
				//mo.asBool = ue4ObjectArray[i]->AsBool();
				continue;
			}
			if (ue4ObjectArray[i]->Type == EJson::Number) {
				mainObject.asNumberArray.Add(ue4ObjectArray[i]->AsNumber());
				//mo.asNumber= ue4ObjectArray[i]->AsNumber();
				continue;
			}
			//mainObject.asMixedArrayObject.Add(mo);
		}
		if (ue4ObjectArray.Num() > 0)
			mainObject.ue4Array = ue4ObjectArray;
	}
	else {
		TSharedPtr<FJsonObject> ue4Object;
		reader = TJsonReaderFactory<>::Create(data);
		successful = FJsonSerializer::Deserialize(reader, ue4Object);
		if (successful) {
			mainObject.ue4Object = ue4Object;
		}
	}



	return mainObject;
}

FSJsonObject UJSONParserBPLibrary::VectorToJsonObject(FVector vector){
	struct FSJsonObject jsonObject;
	jsonObject.ue4Object = MakeShareable(new FJsonObject());
	jsonObject.ue4Object->SetNumberField("X", vector.X);
	jsonObject.ue4Object->SetNumberField("Y", vector.Y);
	jsonObject.ue4Object->SetNumberField("Z", vector.Z);

	return jsonObject;
}

FVector UJSONParserBPLibrary::JsonObjectToVector(FSJsonObject jsonObject){
	FVector vector;
	if (jsonObject.ue4Object != nullptr) {
		vector.X = jsonObject.ue4Object->GetNumberField("X");
		vector.Y = jsonObject.ue4Object->GetNumberField("Y");
		vector.Z = jsonObject.ue4Object->GetNumberField("Z");
	}
	return vector;
}

FSJsonObject UJSONParserBPLibrary::RotatorToJsonObject(FRotator rotator){
	struct FSJsonObject jsonObject;
	jsonObject.ue4Object = MakeShareable(new FJsonObject());
	jsonObject.ue4Object->SetNumberField("Pitch", rotator.Pitch);
	jsonObject.ue4Object->SetNumberField("Yaw", rotator.Yaw);
	jsonObject.ue4Object->SetNumberField("Roll", rotator.Roll);
	

	return jsonObject;
}

FRotator UJSONParserBPLibrary::JsonObjectToRotator(FSJsonObject jsonObject){
	FRotator rotator;
	if (jsonObject.ue4Object != nullptr) {
		rotator.Pitch = jsonObject.ue4Object->GetNumberField("Pitch");
		rotator.Yaw = jsonObject.ue4Object->GetNumberField("Yaw");
		rotator.Roll = jsonObject.ue4Object->GetNumberField("Roll");
	}
	return rotator;
}

FSJsonObject UJSONParserBPLibrary::TransformToJsonObject(FTransform transform){
	struct FSJsonObject jsonObject;
	jsonObject.ue4Object = MakeShareable(new FJsonObject());

	TSharedPtr<FJsonObject>	rotationsObject = MakeShareable(new FJsonObject());
	rotationsObject->SetNumberField("X", transform.GetRotation().X);
	rotationsObject->SetNumberField("Y", transform.GetRotation().Y);
	rotationsObject->SetNumberField("Z", transform.GetRotation().Z);
	rotationsObject->SetNumberField("W", transform.GetRotation().W);

	struct FSJsonObject locationObject = VectorToJsonObject(transform.GetLocation());
	struct FSJsonObject scaleObject = VectorToJsonObject(transform.GetScale3D());

	jsonObject.ue4Object->SetObjectField("Rotation", rotationsObject);
	jsonObject.ue4Object->SetObjectField("Translation", locationObject.ue4Object);
	jsonObject.ue4Object->SetObjectField("Scale3D", scaleObject.ue4Object);


	return jsonObject;
}

FTransform UJSONParserBPLibrary::JsonObjectToTransform(FSJsonObject jsonObject){
	FTransform transform;
	if (jsonObject.ue4Object != nullptr) {

		if (jsonObject.ue4Object->GetObjectField("Translation") != nullptr) {
			TSharedPtr<FJsonObject>	jsonVector = jsonObject.ue4Object->GetObjectField("Translation");
			FVector vector;
			vector.X = jsonVector->GetNumberField("X");
			vector.Y = jsonVector->GetNumberField("Y");
			vector.Z = jsonVector->GetNumberField("Z");
			transform.SetLocation(vector);
		}
		if (jsonObject.ue4Object->GetObjectField("Scale3D") != nullptr) {
			TSharedPtr<FJsonObject>	jsonVector = jsonObject.ue4Object->GetObjectField("Scale3D");
			FVector vector;
			vector.X = jsonVector->GetNumberField("X");
			vector.Y = jsonVector->GetNumberField("Y");
			vector.Z = jsonVector->GetNumberField("Z");
			transform.SetScale3D(vector);
		}

		if (jsonObject.ue4Object->GetObjectField("Rotation") != nullptr) {
			TSharedPtr<FJsonObject>	jsonRotator = jsonObject.ue4Object->GetObjectField("Rotation");
			FQuat rotation;
			rotation.X = jsonRotator->GetNumberField("X");
			rotation.Y = jsonRotator->GetNumberField("Y");
			rotation.Z = jsonRotator->GetNumberField("Z");
			rotation.W = jsonRotator->GetNumberField("W");
			transform.SetRotation(rotation);
		}
	}

	return transform;
}

void UJSONParserBPLibrary::setStructJsonSettings(EJSONJsonStructMapSetting map)
{
	UJSONParserBPLibrary::getJSONParserBPLibrary()->mapSetting = map;
}





void UJSONParserBPLibrary::BreakJson_Object(const FSJsonObject JSONObject, TMap<FString, FSJsonObject>& ObjectMap, TMap<FString, bool>& BoolMap, TMap<FString, float>& NumberMap, TMap<FString, FString>& StringMap, TArray<FSJsonObject>& ObjectArray, TArray<bool>& BoolArray, TArray<float>& NumberArray, TArray<FString>& StringArray) {

	ObjectMap.Empty();
	BoolMap.Empty();
	NumberMap.Empty();
	StringMap.Empty();
	ObjectArray.Empty();
	BoolArray.Empty();
	NumberArray.Empty();
	StringArray.Empty();

	TSharedPtr<FJsonObject> ue4Object = JSONObject.ue4Object;
	if (ue4Object.IsValid()) {
		//StringMap = JSONObject.asStringMap;
		TMap <FString, TSharedPtr<FJsonValue>> values = ue4Object->Values;
		TArray<FString> keys;
		int32 keyCount = values.GetKeys(keys);
		for (int32 i = 0; i < keys.Num(); i++) {
			if (ue4Object->HasTypedField<EJson::Object>(keys[i])) {
				FSJsonObject o;
				o.ue4Object = ue4Object->GetObjectField(keys[i]);
				ObjectMap.Add(keys[i], o);
				continue;
			}
			if (ue4Object->HasTypedField<EJson::String>(keys[i])) {
				StringMap.Add(keys[i], ue4Object->GetStringField(keys[i]));
				continue;
			}
			if (ue4Object->HasTypedField<EJson::Boolean>(keys[i])) {
				BoolMap.Add(keys[i], ue4Object->GetBoolField(keys[i]));
				continue;
			}
			if (ue4Object->HasTypedField<EJson::Number>(keys[i])) {
				NumberMap.Add(keys[i], ue4Object->GetNumberField(keys[i]));
				continue;
			}
			if (ue4Object->HasTypedField<EJson::Array>(keys[i])) {
				//UE_LOG(LogTemp, Warning, TEXT("Oh no, its an array"));
				FSJsonObject o;
				o = generateFSJsonObjectAndArray(o, ue4Object->GetArrayField(keys[i]));
				ObjectMap.Add(keys[i], o);
				continue;
			}
		}
	}


	ObjectArray = JSONObject.asObjectArray;
	BoolArray = JSONObject.asBoolArray;
	NumberArray = JSONObject.asNumberArray;
	StringArray = JSONObject.asStringArray;

}

void UJSONParserBPLibrary::BreakMixedJSONArray(const int32 index, const FSJsonObject JSONObject, int32& length, EJsonDataType& type, FSJsonObject& Object, bool& Bool, float& Number, FString& String) {

	length = 0;
	Object = {};
	Bool = false;
	Number = 0;
	String = "";

	if (JSONObject.ue4Array.Num() == 0) {
		length = 0;
		return;
	}

	length = JSONObject.ue4Array.Num() - 1;
	//UE_LOG(LogTemp, Warning, TEXT("loop:%i"), index);

	if (JSONObject.ue4Array[index]->Type == EJson::Object) {
		Object.ue4Object = JSONObject.ue4Array[index]->AsObject();
		type = EJsonDataType::E_Object;
		return;
	}
	if (JSONObject.ue4Array[index]->Type == EJson::String) {
		String = JSONObject.ue4Array[index]->AsString();
		type = EJsonDataType::E_String;
		return;
	}
	if (JSONObject.ue4Array[index]->Type == EJson::Boolean) {
		Bool = JSONObject.ue4Array[index]->AsBool();
		type = EJsonDataType::E_Bool;
		return;
	}
	if (JSONObject.ue4Array[index]->Type == EJson::Number) {
		Number = JSONObject.ue4Array[index]->AsNumber();
		type = EJsonDataType::E_Number;
		return;
	}

}

FSJsonObject UJSONParserBPLibrary::MakeMixedJSONArray(const FSJsonObject oldJSONObject, EJsonDataType type, FSJsonObject Object, bool Bool, float Number, FString String) {


	FSJsonObject mainObject;

	if (type == EJsonDataType::E_String) {
		mainObject.ue4Array.Add(MakeShareable(new FJsonValueString(String)));
	}
	if (type == EJsonDataType::E_Number) {
		mainObject.ue4Array.Add(MakeShareable(new FJsonValueNumber(Number)));
	}
	if (type == EJsonDataType::E_Bool) {
		mainObject.ue4Array.Add(MakeShareable(new FJsonValueBoolean(Bool)));
	}
	if (type == EJsonDataType::E_Object) {
		mainObject.ue4Array.Add(MakeShareable(new FJsonValueObject(Object.ue4Object)));
	}

	if (oldJSONObject.ue4Array.Num() > 0) {
		TArray< TSharedPtr<FJsonValue> > ue4Array;
		ue4Array.Append(oldJSONObject.ue4Array);
		ue4Array.Append(mainObject.ue4Array);
		mainObject.ue4Array = ue4Array;
	}
	if (oldJSONObject.ue4Object.IsValid()) {
		mainObject.ue4Object = oldJSONObject.ue4Object;
	}
	else {
		mainObject.ue4Object = MakeShareable(new FJsonObject());
	}
	return mainObject;
}





struct FSJsonObject UJSONParserBPLibrary::generateFSJsonObjectAndArray(FSJsonObject mainObject, TArray< TSharedPtr<FJsonValue> > ue4ObjectArray) {

	for (int32 i = 0; i < ue4ObjectArray.Num(); i++) {

		if (ue4ObjectArray[i]->Type == EJson::Object) {
			FSJsonObject o;
			o.ue4Object = ue4ObjectArray[i]->AsObject();
			mainObject.asObjectArray.Add(o);
			continue;
		}

		if (ue4ObjectArray[i]->Type == EJson::String) {
			mainObject.asStringArray.Add(ue4ObjectArray[i]->AsString());
			continue;
		}
		if (ue4ObjectArray[i]->Type == EJson::Boolean) {
			mainObject.asBoolArray.Add(ue4ObjectArray[i]->AsBool());
			continue;
		}
		if (ue4ObjectArray[i]->Type == EJson::Number) {
			mainObject.asNumberArray.Add(ue4ObjectArray[i]->AsNumber());
			continue;
		}
		if (ue4ObjectArray[i]->Type == EJson::Array) {
			FSJsonObject o;
			o = generateFSJsonObjectAndArray(o, ue4ObjectArray[i]->AsArray());
			mainObject.asObjectArray.Add(o);
			continue;
		}
	}

	return mainObject;
}

void UJSONParserBPLibrary::FillStructWithJson(FString structID, FSJsonObject json){
	FSFillStructWithJsonContainer* fillStructWithJsonContainer = UJSONParserBPLibrary::getJSONParserBPLibrary()->getStruct(structID);
	if (fillStructWithJsonContainer != nullptr) {

		UJSONParserBPLibrary::getJSONParserBPLibrary()->removeStruct(structID);
		if (fillStructWithJsonContainer->structProperty != nullptr && fillStructWithJsonContainer->structValue != nullptr) {
			FStructProperty* StructProperty = CastField<FStructProperty>(fillStructWithJsonContainer->structProperty);
			if (StructProperty) {
				for (TFieldIterator<FProperty> PropertyIt(StructProperty->Struct); PropertyIt; ++PropertyIt) {
					for (int32 ArrayIndex = 0; ArrayIndex < PropertyIt->ArrayDim; ArrayIndex++) {
						void* ValuePtr = PropertyIt->ContainerPtrToValuePtr<void>(fillStructWithJsonContainer->structValue, ArrayIndex);

						fillStructToFSJsonObject(*PropertyIt, ValuePtr, json.ue4Object);
					}
				}
			}
		}
	}
}

void UJSONParserBPLibrary::storeStruct(FString id, struct FSFillStructWithJsonContainer fillStructWithJson){
	fillStructMap.Add(id, fillStructWithJson);
}

 FSFillStructWithJsonContainer* UJSONParserBPLibrary::getStruct(FString id){
	return fillStructMap.Find(id);
}

void UJSONParserBPLibrary::removeStruct(FString id){
	fillStructMap.Remove(id);
}

struct FSJsonObject UJSONParserBPLibrary::parseStructToFSJsonObject(FProperty* Property, void* StructPtr) {


	FSJsonObject jsonObj;
	jsonObj.ue4Object = MakeShareable(new FJsonObject());


	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Property->GetAuthoredName());
	FStructProperty* StructProperty = CastField<FStructProperty>(Property);
	if (StructProperty){
		for (TFieldIterator<FProperty> PropertyIt(StructProperty->Struct); PropertyIt; ++PropertyIt){
			for (int32 ArrayIndex = 0; ArrayIndex < PropertyIt->ArrayDim; ArrayIndex++){
				void* ValuePtr = PropertyIt->ContainerPtrToValuePtr<void>(StructPtr, ArrayIndex);
				parseStructPropertyFSJsonObject(*PropertyIt, ValuePtr, jsonObj);
			}
		}
	}

	return jsonObj;
}

void UJSONParserBPLibrary::parseStructPropertyFSJsonObject(FProperty* Property, void* ValuePtr, struct FSJsonObject& jsonObject, bool isArray){

	if (FByteProperty* ByteProperty = CastField<FByteProperty>(Property)) {

		if (isArray) {
			jsonObject.ue4Array.Add(MakeShareable(new FJsonValueNumber(ByteProperty->GetPropertyValue(ValuePtr))));
		}
		else {
			jsonObject.ue4Object->SetNumberField(authoredNameToFString(Property->GetAuthoredName()), ByteProperty->GetPropertyValue(ValuePtr));
		}
		return;
	}

	if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property)){
		if (isArray) {
			if (NumericProperty->IsFloatingPoint()) {
				jsonObject.ue4Array.Add(MakeShareable(new FJsonValueNumber(NumericProperty->GetFloatingPointPropertyValue(ValuePtr))));
			}
			else if (NumericProperty->IsInteger()) {
				jsonObject.ue4Array.Add(MakeShareable(new FJsonValueNumber(NumericProperty->GetSignedIntPropertyValue(ValuePtr))));
			}
		}
		else {
			if (NumericProperty->IsFloatingPoint()) {
				jsonObject.ue4Object->SetNumberField(authoredNameToFString(Property->GetAuthoredName()), NumericProperty->GetFloatingPointPropertyValue(ValuePtr));
			}
			else if (NumericProperty->IsInteger()) {
				jsonObject.ue4Object->SetNumberField(authoredNameToFString(Property->GetAuthoredName()), NumericProperty->GetSignedIntPropertyValue(ValuePtr));
			}
		}
		return;
	}

	if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property)){
		if (isArray) {
			jsonObject.ue4Array.Add(MakeShareable(new FJsonValueBoolean(BoolProperty->GetPropertyValue(ValuePtr))));
		}
		else {
			jsonObject.ue4Object->SetBoolField(authoredNameToFString(Property->GetAuthoredName()), BoolProperty->GetPropertyValue(ValuePtr));
		}
		
		return;
	}

	if (FNameProperty* NameProperty = CastField<FNameProperty>(Property)){
		if (isArray) {
			jsonObject.ue4Array.Add(MakeShareable(new FJsonValueString(NameProperty->GetPropertyValue(ValuePtr).ToString())));
		}
		else {
			jsonObject.ue4Object->SetStringField(authoredNameToFString(Property->GetAuthoredName()), NameProperty->GetPropertyValue(ValuePtr).ToString());
		}
		return;
	}

	if (FTextProperty* TextProperty = CastField<FTextProperty>(Property)) {
		if (isArray) {
			jsonObject.ue4Array.Add(MakeShareable(new FJsonValueString(TextProperty->GetPropertyValue(ValuePtr).ToString())));
		}
		else {
			jsonObject.ue4Object->SetStringField(authoredNameToFString(Property->GetAuthoredName()), TextProperty->GetPropertyValue(ValuePtr).ToString());
		}
		return;
	}

	if (FStrProperty* StringProperty = CastField<FStrProperty>(Property)){
		if (isArray) {
			jsonObject.ue4Array.Add(MakeShareable(new FJsonValueString(StringProperty->GetPropertyValue(ValuePtr))));
		}
		else {
			jsonObject.ue4Object->SetStringField(authoredNameToFString(Property->GetAuthoredName()), StringProperty->GetPropertyValue(ValuePtr));
		}
		return;
	}

	if (FSetProperty* SetProperty = CastField<FSetProperty>(Property)) {

		FSJsonObject jsonSubObj;
		jsonSubObj.ue4Object = MakeShareable(new FJsonObject());

		FScriptSetHelper Helper(SetProperty, ValuePtr);
		for (int32 i = 0, n = Helper.Num(); i < n; ++i) {
			parseStructPropertyFSJsonObject(SetProperty->ElementProp, Helper.GetElementPtr(i), jsonSubObj, true);
		}

		//Set Array mix does not work via the UI and is therefore not implement

		jsonObject.ue4Object->SetArrayField(authoredNameToFString(Property->GetAuthoredName()), jsonSubObj.ue4Array);
		
		return;
	}


	if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property)){   

		FSJsonObject jsonSubObj;
		jsonSubObj.ue4Object = MakeShareable(new FJsonObject());

		FScriptArrayHelper Helper(ArrayProperty, ValuePtr);
		for (int32 i = 0, n = Helper.Num(); i < n; ++i) {
			parseStructPropertyFSJsonObject(ArrayProperty->Inner, Helper.GetRawPtr(i), jsonSubObj, true);
		}
			   
		//Array in array does not work via the UI and is therefore not implemented
				
		jsonObject.ue4Object->SetArrayField(authoredNameToFString(Property->GetAuthoredName()), jsonSubObj.ue4Array);
		
		return;
	}


	if (FMapProperty* MapProperty = CastField<FMapProperty>(Property)) {

		FSJsonObject jsonMainMapObj;
		jsonMainMapObj.ue4Object = MakeShareable(new FJsonObject());

		FScriptMapHelper Helper(MapProperty, ValuePtr);
		for (int32 i = 0, n = Helper.Num(); i < n; ++i) {
			FSJsonObject jsonSubMapObj;
			jsonSubMapObj.ue4Object = MakeShareable(new FJsonObject());


			parseStructPropertyFSJsonObject(Helper.GetKeyProperty(), Helper.GetKeyPtr(i), jsonSubMapObj, true);
			parseStructPropertyFSJsonObject(Helper.GetValueProperty(), Helper.GetValuePtr(i), jsonSubMapObj, true);
		
			if (UJSONParserBPLibrary::getJSONParserBPLibrary()->mapSetting == EJSONJsonStructMapSetting::E_object) {

				if (jsonSubMapObj.ue4Array.Num() == 2 && jsonSubMapObj.ue4Array[0]->Type == EJson::String) {
					switch (jsonSubMapObj.ue4Array[1]->Type)
					{
					case EJson::Object:
						jsonMainMapObj.ue4Object->SetObjectField(authoredNameToFString(jsonSubMapObj.ue4Array[0]->AsString()), jsonSubMapObj.ue4Array[1]->AsObject());
						break;
					case EJson::String:
						jsonMainMapObj.ue4Object->SetStringField(authoredNameToFString(jsonSubMapObj.ue4Array[0]->AsString()), jsonSubMapObj.ue4Array[1]->AsString());
						break;
					case EJson::Boolean:
						jsonMainMapObj.ue4Object->SetBoolField(authoredNameToFString(jsonSubMapObj.ue4Array[0]->AsString()), jsonSubMapObj.ue4Array[1]->AsBool());
						break;
					case EJson::Number:
						jsonMainMapObj.ue4Object->SetNumberField(authoredNameToFString(jsonSubMapObj.ue4Array[0]->AsString()), jsonSubMapObj.ue4Array[1]->AsNumber());
						break;
					default:
						FString s;
						if (jsonSubMapObj.ue4Array[1]->TryGetString(s)) {
							jsonMainMapObj.ue4Object->SetStringField(authoredNameToFString(jsonSubMapObj.ue4Array[0]->AsString()),s);
						}
						break;
					}
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("JSONParser: Map should be saved as a json object but the key (%s) is not a string. Record is skipped. Check setStructJsonSettings."), *authoredNameToFString(Helper.GetKeyProperty()->GetAuthoredName()));
					continue;
				}
			}
			else {
			jsonMainMapObj.ue4Array.Add(MakeShareable(new FJsonValueArray(jsonSubMapObj.ue4Array)));
		}


		}

		if (UJSONParserBPLibrary::getJSONParserBPLibrary()->mapSetting == EJSONJsonStructMapSetting::E_object) {
			if (isArray) {
				jsonObject.ue4Array.Add(MakeShareable(new FJsonValueObject(jsonMainMapObj.ue4Object)));
			}
			else {
				jsonObject.ue4Object->SetObjectField(authoredNameToFString(Property->GetAuthoredName()), jsonMainMapObj.ue4Object);
			}
		}
		else {
		if (isArray) {
			jsonObject.ue4Array.Add(MakeShareable(new FJsonValueArray(jsonMainMapObj.ue4Array)));
		}
		else {
			jsonObject.ue4Object->SetArrayField(authoredNameToFString(Property->GetAuthoredName()), jsonMainMapObj.ue4Array);
		}
		}



		
		return;
	}

	//See deserialization (fillStructToFSJsonObject) explanation.

	if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property)) {
		UE_LOG(LogTemp, Warning, TEXT("JSONParser: '%s' was skipped. Object references are currently not supported. Possibly in the future certain types (Materials, Actors..) will be supported. "), *Property->GetAuthoredName());

	//	UObject* obj = ObjectProperty->GetPropertyValue(ValuePtr);
	//	TArray<uint8> bytes;
	//	FObjectWriter Ar = FObjectWriter(bytes);
	//	obj->Serialize(Ar);

	//	if (isArray) {
	//		//UE_LOG(LogTemp, Display, TEXT("JSONParser: FObjectProperty (Array): %s"), *Property->GetAuthoredName());
	//		jsonObject.ue4Array.Add(MakeShareable(new FJsonValueString(FBase64::Encode(bytes))));
	//	}
	//	else {
	//		//UE_LOG(LogTemp, Display, TEXT("JSONParser: FObjectProperty: %s"), *Property->GetAuthoredName());
	//		jsonObject.ue4Object->SetStringField(Property->GetAuthoredName(), FBase64::Encode(bytes));
	//	}
	//	
		return;
	}


	if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property)) {
		if (isArray) {
			jsonObject.ue4Array.Add(MakeShareable(new FJsonValueNumber(*(int64*)ValuePtr)));
		}
		else {
			jsonObject.ue4Object->SetNumberField(authoredNameToFString(Property->GetAuthoredName()),*(int64*)ValuePtr);
		}

		return;
	}

	
	if (FStructProperty* StructProperty = CastField<FStructProperty>(Property)) {

		FSJsonObject subJsonObject = parseStructToFSJsonObject(Property, ValuePtr);
		if (subJsonObject.ue4Object != nullptr) {
			if (isArray) {
				jsonObject.ue4Array.Add(MakeShareable(new FJsonValueObject(subJsonObject.ue4Object)));
			}
			else {
				jsonObject.ue4Object->SetObjectField(authoredNameToFString(Property->GetAuthoredName()), subJsonObject.ue4Object);
			}
		}
		return;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("JSONParser: '%s' was not merged into JSON because it is an unknown / not supported data type. "), *Property->GetAuthoredName());
	}
}



void UJSONParserBPLibrary::fillStructToFSJsonObject(FProperty* Property, void* ValuePtr, TSharedPtr<FJsonObject> ue4Object, TSharedPtr<FJsonValue> arrayValue){

	if (FByteProperty* ByteProperty = CastField<FByteProperty>(Property)) {
		
		if (arrayValue != nullptr) {
			ByteProperty->SetPropertyValue(ValuePtr, (uint8)arrayValue->AsNumber());
		}
		else {
			if (ue4Object) {
				FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
				if (ue4Object->HasTypedField<EJson::Number>(authoredName)) {
					ByteProperty->SetPropertyValue(ValuePtr, (uint8)ue4Object->GetNumberField(authoredName));
				}
			}
		}

		return;
	}


	if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property)) {

		if (arrayValue != nullptr) {
			if (NumericProperty->IsFloatingPoint()) {
				NumericProperty->SetFloatingPointPropertyValue(ValuePtr, arrayValue->AsNumber());
			}
			else if (NumericProperty->IsInteger()) {
				NumericProperty->SetIntPropertyValue(ValuePtr, (int64)arrayValue->AsNumber());
			}
		}
		else {
			if (ue4Object != nullptr) {
				FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
				if (ue4Object->HasTypedField<EJson::Number>(authoredName)) {
					if (NumericProperty->IsFloatingPoint()) {
						NumericProperty->SetFloatingPointPropertyValue(ValuePtr, ue4Object->GetNumberField(authoredName));
					}
					else if (NumericProperty->IsInteger()) {
						NumericProperty->SetIntPropertyValue(ValuePtr, (int64)ue4Object->GetNumberField(authoredName));
					}
				}
			}
		}
		return;
	}

	if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property)) {
		if (arrayValue != nullptr) {
			BoolProperty->SetPropertyValue(ValuePtr, arrayValue->AsBool());
		}
		else {
			if (ue4Object) {
				FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
				if (ue4Object->HasTypedField<EJson::Boolean>(authoredName)) {
					BoolProperty->SetPropertyValue(ValuePtr, ue4Object->GetBoolField(authoredName));
				}
			}
		}

		return;
	}

	if (FNameProperty* NameProperty = CastField<FNameProperty>(Property)) {
		if (arrayValue != nullptr) {
			NameProperty->SetPropertyValue(ValuePtr, FName(arrayValue->AsString()));
		}
		else {
			if (ue4Object) {
				FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
				if (ue4Object->HasTypedField<EJson::String>(authoredName)) {
					NameProperty->SetPropertyValue(ValuePtr, FName(ue4Object->GetStringField(authoredName)));
				}
			}
		}
		return;
	}

	if (FTextProperty* TextProperty = CastField<FTextProperty>(Property)) {
		if (arrayValue != nullptr) {
			TextProperty->SetPropertyValue(ValuePtr, FText::FromString(arrayValue->AsString()));
		}
		else {
			if (ue4Object) {
				FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
				if (ue4Object->HasTypedField<EJson::String>(authoredName)) {
					TextProperty->SetPropertyValue(ValuePtr, FText::FromString(ue4Object->GetStringField(authoredName)));
				}
			}
		}
		return;
	}

	if (FStrProperty* StringProperty = CastField<FStrProperty>(Property)) {
		if (arrayValue != nullptr) {
			StringProperty->SetPropertyValue(ValuePtr, arrayValue->AsString());
		}
		else {
			if (ue4Object) {
				FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
				if (ue4Object->HasTypedField<EJson::String>(authoredName)) {
					StringProperty->SetPropertyValue(ValuePtr, ue4Object->GetStringField(authoredName));
				}
			}
		}

		return;
	}



	if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property)) {

	
		//Array in array does not work via the UI and is therefore not implemented
		
		if (ue4Object) {
			FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
			if (ue4Object->HasTypedField<EJson::Array>(authoredName)) {
				TArray< TSharedPtr<FJsonValue> > ue4Array = ue4Object->GetArrayField(authoredName);
				if (ue4Array.Num() > 0) {
					FScriptArrayHelper ArrayPropertyHelper(ArrayProperty, ValuePtr);
					ArrayPropertyHelper.EmptyValues();
					ArrayPropertyHelper.AddValues(ue4Array.Num());
					for (int32 i = 0; i < ue4Array.Num(); i++) {
						if (ue4Array[i] != nullptr) {
							fillStructToFSJsonObject(ArrayProperty->Inner, ArrayPropertyHelper.GetRawPtr(i), nullptr, ue4Array[i]);
						}
					}
				}
			}
		}
		
		return;
	}
	
	if (FSetProperty* SetProperty = CastField<FSetProperty>(Property)) {

		//Set Array mix does not work via the UI and is therefore not implement
	
		if (ue4Object) {
			FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
			if (ue4Object->HasTypedField<EJson::Array>(authoredName)) {
				TArray< TSharedPtr<FJsonValue> > ue4Array = ue4Object->GetArrayField(authoredName);
				if (ue4Array.Num() > 0) {
					FScriptSetHelper SetPropertyHelper(SetProperty, ValuePtr);
					SetPropertyHelper.EmptyElements();
					for (int32 i = 0; i < ue4Array.Num(); i++) {
						if (ue4Array[i] != nullptr) {
							int32 valPos = SetPropertyHelper.AddDefaultValue_Invalid_NeedsRehash();
							fillStructToFSJsonObject(SetProperty->ElementProp, SetPropertyHelper.GetElementPtr(valPos), nullptr, ue4Array[i]);
						}
					}
					SetPropertyHelper.Rehash();
				}
			}
		}
	
		return;
	}

	if (FMapProperty* MapProperty = CastField<FMapProperty>(Property)) {

		//Map Array mix does not work via the UI and is therefore not implement

		if (ue4Object) {
			FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
			if (ue4Object->HasTypedField<EJson::Array>(authoredName)) {
				FScriptMapHelper Helper(MapProperty, ValuePtr);
				Helper.EmptyValues();
				TArray< TSharedPtr<FJsonValue> > ue4Array = ue4Object->GetArrayField(authoredName);
				for (int32 i = 0; i < ue4Array.Num(); i++) {
					TArray< TSharedPtr<FJsonValue> > ue4SubArray = ue4Array[i]->AsArray();
					if (ue4SubArray.Num() == 2) {
						
						Helper.AddDefaultValue_Invalid_NeedsRehash();
						fillStructToFSJsonObject(Helper.GetKeyProperty(), Helper.GetKeyPtr(i), nullptr, ue4SubArray[0]);
						fillStructToFSJsonObject(Helper.GetValueProperty(), Helper.GetValuePtr(i), nullptr, ue4SubArray[1]);

					}
				}
				Helper.Rehash();
			}
			else {
				if (UJSONParserBPLibrary::getJSONParserBPLibrary()->mapSetting == EJSONJsonStructMapSetting::E_object) {
					if (ue4Object->HasTypedField<EJson::Object>(authoredName)) {

						int32 i = 0;
						FScriptMapHelper Helper(MapProperty, ValuePtr);
						Helper.EmptyValues();
						TArray< TSharedPtr<FJsonValue>> ue4SubArray;
						for (auto& element : ue4Object->GetObjectField(authoredName)->Values) {
							//UE_LOG(LogTemp, Warning, TEXT("JSONParser: key %s. "), *element.Key);

							//transfer to an array for compatibility with the existing code
							ue4SubArray.Empty();
							ue4SubArray.Add(MakeShareable(new FJsonValueString(fStringToAuthoredName(element.Key))));
							
														
							switch (element.Value->Type)
							{
							case EJson::Object:
								ue4SubArray.Add(MakeShareable(new FJsonValueObject(element.Value->AsObject())));
								break;
							case EJson::String:
								ue4SubArray.Add(MakeShareable(new FJsonValueString(element.Value->AsString())));
								break;
							case EJson::Boolean:
								ue4SubArray.Add(MakeShareable(new FJsonValueBoolean(element.Value->AsBool())));
								break;
							case EJson::Number:
								ue4SubArray.Add(MakeShareable(new FJsonValueNumber(element.Value->AsNumber())));
								break;
							default:
		
								break;
							}
							if (ue4SubArray.Num() != 2) {
								continue;
							}

														
							Helper.AddDefaultValue_Invalid_NeedsRehash();
							fillStructToFSJsonObject(Helper.GetKeyProperty(), Helper.GetKeyPtr(i), nullptr, ue4SubArray[0]);
							fillStructToFSJsonObject(Helper.GetValueProperty(), Helper.GetValuePtr(i), nullptr, ue4SubArray[1]);
							i++;
						}
						Helper.Rehash();
					}
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("JSONParser: Can't parse map %s. Record is skipped. Check setStructJsonSettings."), *authoredName);
				}
			}
		}
		return;
	}

	//You cannot create an instance of a UObject. You have to use one of the children and for that you have to specify the type (Actor, Material...) explicitly. That's why this is not implied for now.

	if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property)) {
		UE_LOG(LogTemp, Warning, TEXT("JSONParser: '%s' was skipped. Object references are currently not supported. Possibly in the future certain types (Materials, Actors..) will be supported. "), *Property->GetAuthoredName());

	//	if (arrayValue != nullptr) {
	//		//StringProperty->SetPropertyValue(ValuePtr, arrayValue->AsString());
	//	}
	//	else {
	//		if (ue4Object) {
	//			if (ue4Object->HasTypedField<EJson::String>(Property->GetAuthoredName())) {
	//				FString base64UObject = ue4Object->GetStringField(Property->GetAuthoredName());
	//				TArray<uint8> bytes;
	//				FBase64::Decode(base64UObject, bytes);
	//				UObject* obj = NewObject<UObject>(UObject::StaticClass());
	//				FObjectReader Ar = FObjectReader(obj,bytes);
	//				ObjectProperty->SetPropertyValue(ValuePtr, obj);
	//			}
	//		}
	//	}
		return;
	}


	if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property)) {

		if (arrayValue != nullptr) {
			EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(ValuePtr, (int64)arrayValue->AsNumber());
		}
		else {
			if (ue4Object) {
				FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
				if (ue4Object->HasTypedField<EJson::Number>(authoredName)) {
					EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(ValuePtr, (int64)ue4Object->GetNumberField(authoredName));
				}
			}
		}
		return;
	}


	if (FStructProperty* StructProperty = CastField<FStructProperty>(Property)) {
		if (arrayValue != nullptr) {
			if (arrayValue->Type == EJson::Object) {
				for (TFieldIterator<FProperty> PropertyIt(StructProperty->Struct); PropertyIt; ++PropertyIt) {
					for (int32 ArrayIndex = 0; ArrayIndex < PropertyIt->ArrayDim; ArrayIndex++) {
						void* valptr = PropertyIt->ContainerPtrToValuePtr<void>(ValuePtr, ArrayIndex);
						fillStructToFSJsonObject(*PropertyIt, valptr, arrayValue->AsObject());
					}
				}
			}
		}
		else {
			if (ue4Object) {
				FString authoredName = fStringToAuthoredName(Property->GetAuthoredName());
				if (ue4Object->HasTypedField<EJson::Object>(authoredName)) {
					FSJsonObject json;
					json.ue4Object = ue4Object->GetObjectField(authoredName);
					for (TFieldIterator<FProperty> PropertyIt(StructProperty->Struct); PropertyIt; ++PropertyIt) {
						for (int32 ArrayIndex = 0; ArrayIndex < PropertyIt->ArrayDim; ArrayIndex++) {
							void* valptr = PropertyIt->ContainerPtrToValuePtr<void>(ValuePtr, ArrayIndex);
							fillStructToFSJsonObject(*PropertyIt, valptr, json.ue4Object);
						}
					}
				}
			}
		}

		return;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("JSONParser: '%s' was not inserted into the structure because it is an unknown / not supported data type. "), *Property->GetAuthoredName());
	}



	return;
}


void UJSONParserBPLibrary::prepareStructAuthoredNames(FProperty* Property) {

	FStructProperty* StructProperty = CastField<FStructProperty>(Property);
	if (StructProperty) {
		for (TFieldIterator<FProperty> PropertyIt(StructProperty->Struct); PropertyIt; ++PropertyIt) {
			if (*PropertyIt != nullptr && *PropertyIt->GetAuthoredName() != nullptr) {
				authoredNameToFString(*PropertyIt->GetAuthoredName());
			}
			/*for (int32 ArrayIndex = 0; ArrayIndex < PropertyIt->ArrayDim; ArrayIndex++) {
				void* ValuePtr = PropertyIt->ContainerPtrToValuePtr<void>(StructPtr, ArrayIndex);
				createStructAuthoredNames(*PropertyIt, ValuePtr);
			}*/
		}
	}
}


FString UJSONParserBPLibrary::authoredNameToFString(FString authoredName) {
	#if WITH_EDITOR | PLATFORM_ANDROID | PLATFORM_IOS
		return authoredName;
	#endif

	TArray<FString> lines;
	int32 lineCount = authoredName.ParseIntoArray(lines, TEXT("_"), false);
	if (lineCount > 2) {
		if (lines[lines.Num() - 1].Len() == 32) {	
			FString name = authoredName;
			name.RemoveFromEnd("_"+lines[lines.Num() - 2] + "_" + lines[lines.Num() - 1]);
			JSONParserBPLibrary->authoredNames.Add(authoredName, name);
			return name;
		}
	}

	return authoredName;
}

FString UJSONParserBPLibrary::fStringToAuthoredName(FString name){
	#if WITH_EDITOR | PLATFORM_ANDROID | PLATFORM_IOS
		return name;
	#endif

	if (JSONParserBPLibrary->authoredNames.Find(name) != nullptr) {
		return *UJSONParserBPLibrary::JSONParserBPLibrary->authoredNames.Find(name);
	}


	return name;
}


void UJSONParserBPLibrary::fileToBase64String(EJSONDirectoryType directoryType, FString filePath, FString& base64String, FString& fileName) {

	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return;
	}


	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}


	if (!FPaths::FileExists(dir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: File not found: %s"), *dir);
		return;
	}

	TArray<uint8> fileData;
	if (!FFileHelper::LoadFileToArray(fileData, *dir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Can't read File: %s"), *dir);
		return;
	}

	fileName = FPaths::GetCleanFilename(dir);
	base64String = FBase64::Encode(fileData);
}

bool UJSONParserBPLibrary::base64StringToFile(EJSONDirectoryType directoryType, FString base64String, FString filePath) {

	if (base64String.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Base64String not set."));
		return false;
	}

	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return false;
	}

	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}

	FString fileName = FPaths::GetCleanFilename(dir);
	FString tmpDir = dir;
	tmpDir.RemoveFromEnd(fileName);
	if (!FPaths::DirectoryExists(tmpDir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Directory not exists: %s"), *tmpDir);
		return false;
	}

	TArray<uint8> fileData;
	if (!FBase64::Decode(*base64String, fileData)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Base64 decode failed: %s"), *dir);
		return false;
	}

	if (!FFileHelper::SaveArrayToFile(fileData, *dir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Can't save File: %s"), *dir);
		return false;
	}

	return FPaths::FileExists(dir);
}

bool UJSONParserBPLibrary::readStringFromFile(EJSONDirectoryType directoryType, FString filePath, FString& JSON_String) {
	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return false;
	}


	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}


	if (!FPaths::FileExists(dir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: File not found: %s"), *dir);
		return false;
	}

	if (!FFileHelper::LoadFileToString(JSON_String, *dir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Can't read File: %s"), *dir);
		return false;
	}
	return true;
}

void UJSONParserBPLibrary::readBytesFromFile(EJSONDirectoryType directoryType, FString filePath, TArray<uint8>& bytes) {
	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return;
	}


	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}


	if (!FPaths::FileExists(dir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: File not found: %s"), *dir);
		return;
	}



	if (!FFileHelper::LoadFileToArray(bytes, *dir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Can't read File: %s"), *dir);
		return;
	}
}

bool UJSONParserBPLibrary::writeStringToFile(EJSONDirectoryType directoryType, FString JSON_String, FString filePath) {
	if (JSON_String.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: JSON String is empty."));
		return false;
	}

	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return false;
	}

	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}

	FString fileName = FPaths::GetCleanFilename(dir);
	FString tmpDir = dir;
	tmpDir.RemoveFromEnd(fileName);
	if (!FPaths::DirectoryExists(tmpDir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Directory not exists: %s"), *tmpDir);
		return false;
	}



	if (!FFileHelper::SaveStringToFile(JSON_String, *dir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Can't save File: %s"), *dir);
		return false;
	}

	return FPaths::FileExists(dir);
}


bool UJSONParserBPLibrary::writeBytesToFile(EJSONDirectoryType directoryType, TArray<uint8> bytes, FString filePath) {
	if (bytes.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: JSON Byte Array is empty."));
		return false;
	}

	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return false;
	}

	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}

	FString fileName = FPaths::GetCleanFilename(dir);
	FString tmpDir = dir;
	tmpDir.RemoveFromEnd(fileName);
	if (!FPaths::DirectoryExists(tmpDir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Directory not exists: %s"), *tmpDir);
		return false;
	}



	if (!FFileHelper::SaveArrayToFile(bytes, *dir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Can't save File: %s"), *dir);
		return false;
	}

	return FPaths::FileExists(dir);
}

bool UJSONParserBPLibrary::deleteFile(EJSONDirectoryType directoryType, FString filePath){
	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return false;
	}

	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}

	FString fileName = FPaths::GetCleanFilename(dir);
	FString tmpDir = dir;
	tmpDir.RemoveFromEnd(fileName);
	if (!FPaths::DirectoryExists(tmpDir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Directory not exists: %s"), *tmpDir);
		return false;
	}
	return FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*dir);
}

bool UJSONParserBPLibrary::createFile(EJSONDirectoryType directoryType, FString filePath){
	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return false;
	}

	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}

	FString fileName = FPaths::GetCleanFilename(dir);
	FString tmpDir = dir;
	tmpDir.RemoveFromEnd(fileName);
	if (!FPaths::DirectoryExists(tmpDir)) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: Directory not exists: %s"), *tmpDir);
		return false;
	}
	delete IPlatformFile::GetPlatformPhysical().OpenWrite(*dir);
	return FPaths::FileExists(*dir);
}

bool UJSONParserBPLibrary::fileExists(EJSONDirectoryType directoryType, FString filePath){
	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return false;
	}

	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}

	return FPaths::FileExists(*dir);
}

bool UJSONParserBPLibrary::dirExists(EJSONDirectoryType directoryType, FString filePath){
	if (filePath.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("JSONParser: FilePath not set."));
		return false;
	}

	FString dir;
	if (directoryType == EJSONDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}

	return FPaths::DirectoryExists(*dir);
}

//Async  tests
////Delegates
//void UJSONParserBPLibrary::JSONParserReadStringFromFileEventDelegate(const FString jsonString, const FString optionalID){}
//void UJSONParserBPLibrary::JSONParserRreadBytesFromFileEventDelegate(const TArray<uint8>& bytes, const FString optionalID){}
//void UJSONParserBPLibrary::JSONParserWriteStringToFileEventDelegate(const bool success, const FString optionalID){}
//void UJSONParserBPLibrary::JSONParserWriteBytesToFileEventDelegate(const bool success, const FString optionalID){}
//
//void UJSONParserBPLibrary::readStringFromFileAsync(EJSONDirectoryType directoryType, FString filePath, FString optionalID, bool switchToGameThread){
//	if (jsonThread == nullptr) {
//		jsonThread = new FJSONParserThread();
//	}
//
//	FSJsonThreadJob* job = new FSJsonThreadJob();
//	job->switchToGameThread = switchToGameThread;
//	job->optionalID = optionalID;
//	job->directoryType = directoryType;
//	job->filePath = filePath;
//	job->type = EJSONThread::E_readStringFromFile;
//
//	jsonThread->addJob(job);
//}
//
//void UJSONParserBPLibrary::readBytesFromFileAsync(EJSONDirectoryType directoryType, FString filePath, FString optionalID, bool switchToGameThread){
//	if (jsonThread == nullptr) {
//		jsonThread = new FJSONParserThread();
//	}
//
//	FSJsonThreadJob* job = new FSJsonThreadJob();
//	job->switchToGameThread = switchToGameThread;
//	job->optionalID = optionalID;
//	job->directoryType = directoryType;
//	job->filePath = filePath;
//	job->type = EJSONThread::E_readBytesFromFile;
//
//	jsonThread->addJob(job);
//}
//
//void UJSONParserBPLibrary::writeStringToFileAsync(EJSONDirectoryType directoryType, FString JSON_String, FString filePath, FString optionalID, bool switchToGameThread){
//	if (jsonThread == nullptr) {
//		jsonThread = new FJSONParserThread();
//	}
//
//	FSJsonThreadJob* job = new FSJsonThreadJob();
//	job->switchToGameThread = switchToGameThread;
//	job->optionalID = optionalID;
//	job->directoryType = directoryType;
//	job->filePath = filePath;
//	job->jsonString = JSON_String;
//	job->type = EJSONThread::E_writeStringToFile;
//
//	jsonThread->addJob(job);
//}
//
//void UJSONParserBPLibrary::writeBytesToFileAsync(EJSONDirectoryType directoryType, TArray<uint8> bytes, FString filePath, FString optionalID, bool switchToGameThread){
//	if (jsonThread == nullptr) {
//		jsonThread = new FJSONParserThread();
//	}
//
//	FSJsonThreadJob* job = new FSJsonThreadJob();
//	job->switchToGameThread = switchToGameThread;
//	job->optionalID = optionalID;
//	job->directoryType = directoryType;
//	job->filePath = filePath;
//	job->bytes = bytes;
//	job->type = EJSONThread::E_writeBytesToFile;
//
//	jsonThread->addJob(job);
//}

void UJSONParserBPLibrary::compressData(EJsonCompressFormat format, TArray<uint8> uncompressedData, TArray<uint8>& compressedData, int32& compressedDataSize, int32& uncompressedDataSize, bool& success){

	FName formatName = NAME_Zlib;
	switch (format)
	{
	case EJsonCompressFormat::E_Zlib:
		formatName = NAME_Zlib;
		break;
	case EJsonCompressFormat::E_Gzip:
		formatName = NAME_Gzip;
		break;
	case EJsonCompressFormat::E_LZ4:
		formatName = NAME_LZ4;
		break;
	}

	uncompressedDataSize = uncompressedData.Num();
	
	if (format == EJsonCompressFormat::E_Gzip) {
		compressedData.AddZeroed(uncompressedDataSize);
	}
	else {
		compressedData.AddZeroed(FCompression::CompressMemoryBound(formatName, uncompressedDataSize));
	}

	if (compressedData.Num() == 0 || uncompressedData.Num() == 0) {
		success = false;
		return;
	}


	compressedDataSize = compressedData.Num();

	success = FCompression::CompressMemory(formatName, compressedData.GetData(), compressedDataSize, uncompressedData.GetData(), uncompressedDataSize);
	compressedData.SetNum(compressedDataSize);
	compressedData.Shrink();
	
}

void UJSONParserBPLibrary::uncompressData(EJsonCompressFormat format, int32 uncompressedDataSize, TArray<uint8> compressedData, TArray<uint8>& uncompressedData, bool& success){
	
	FName formatName = NAME_Zlib;
	switch (format)
	{
	case EJsonCompressFormat::E_Zlib:
		formatName = NAME_Zlib;
		break;
	case EJsonCompressFormat::E_Gzip:
		formatName = NAME_Gzip;
		break;
	case EJsonCompressFormat::E_LZ4:
		formatName = NAME_LZ4;
		break;
	}

	uncompressedData.AddZeroed(uncompressedDataSize);

	if (compressedData.Num() == 0 || uncompressedData.Num() == 0) {
		success = false;
		return;
	}


	success = FCompression::UncompressMemory(formatName, uncompressedData.GetData(), uncompressedDataSize, compressedData.GetData(), compressedData.Num());

}

TArray<uint8> UJSONParserBPLibrary::stringToByteArray(FString string){
	FTCHARToUTF8 Convert(*string);
	TArray<uint8> byteArray;
	byteArray.AddUninitialized(Convert.Length());
	FMemory::Memcpy(byteArray.GetData(), Convert.Get(), Convert.Length());
	return byteArray;
}

FString UJSONParserBPLibrary::byteArrayToString(TArray<uint8> byteArray){
	if (byteArray.Num() == 0){
		return FString();
	}
	if (byteArray.Last() != 0x00) {
		byteArray.Add(0x00);// null-terminator
	}
	char* Data = (char*)byteArray.GetData();
	return FString(UTF8_TO_TCHAR(Data));
}
