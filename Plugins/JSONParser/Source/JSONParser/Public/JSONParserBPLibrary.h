// Copyright 2017-2021 David Romanski(Socke). All Rights Reserved.

#pragma once

#include "JSONParser.h"
#include "JSONParserBPLibrary.generated.h"

//class FJSONParserThread;





UCLASS()
class JSONPARSER_API UJSONParserBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()


public:

	static UJSONParserBPLibrary* JSONParserBPLibrary;
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON")
		static UJSONParserBPLibrary* getJSONParserBPLibrary();

	//find variables
	//UFUNCTION(BlueprintCallable, Category = "JSON")
	//	static void findStringInJsonObject(struct FSJsonObject jsonObject, FString key, bool searchInArrayObjects, bool printLogs, FString& value);
	//static void findStringInternalInJsonObject(TSharedPtr<FJsonObject> jsonObject, FString key, bool searchInArrayObjects, bool printLogs, FString& value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Make")
		static TMap<FString, FString> MakeMapString(FString Key, FString Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Make")
		static TMap<FString, float> MakeMapNumber(FString Key, float Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Make")
		static TMap<FString, bool> MakeMapBool(FString Key, bool Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Make")
		static TMap<FString, FSJsonObject> MakeMapObject(FString Key, FSJsonObject Value);

	//add elements to tmap
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Add")
		static TMap<FString, FString> AddMapString(TMap<FString, FString> map, FString Key, FString Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Add")
		static TMap<FString, float> AddMapNumber(TMap<FString, float> map, FString Key, float Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Add")
		static TMap<FString, bool> AddMapBool(TMap<FString, bool> map, FString Key, bool Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Add")
		static TMap<FString, FSJsonObject> AddMapObject(TMap<FString, FSJsonObject> map, FString Key, FSJsonObject Value);

	//change values in tmap
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Change")
		static TMap<FString, FString> ChangeMapString(TMap<FString, FString> map, FString Key, FString Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Change")
		static TMap<FString, float> ChangeMapNumber(TMap<FString, float> map, FString Key, float Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Change")
		static TMap<FString, bool> ChangeMapBool(TMap<FString, bool> map, FString Key, bool Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Change")
		static TMap<FString, FSJsonObject> ChangeMapObject(TMap<FString, FSJsonObject> map, FString Key, FSJsonObject Value);

	//create variables
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Make")
		static FString MakeString(FString Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Make")
		static float MakeNumber(float Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JSON|Make")
		static bool MakeBool(bool Value);



	//json stuff
	UFUNCTION(BlueprintCallable, Category = "JSON", meta = (AutoCreateRefTerm = "ObjectMap,BoolMap,NumberMap,StringMap,ObjectArray,BoolArray,NumberArray,StringArray"))
		static struct FSJsonObject MakeJson_Object(TMap<FString, FSJsonObject> ObjectMap, TMap<FString, bool> BoolMap, TMap<FString, float> NumberMap, TMap<FString, FString> StringMap, TArray<FSJsonObject> ObjectArray, TArray<bool> BoolArray, TArray<float> NumberArray, TArray<FString> StringArray);


	/**
	*Create a JSON String from a JSON Object.
	*@param mainObject JSON Object
	*@param removeLineBreaks removes \r, \n and \t
	*@param addPadding If there is truncated JSON padding in the JSON object, it is appended again.
	*/
	UFUNCTION(BlueprintCallable, Category = "JSON")
		static FString JsonObjectToString(bool& successful, struct FSJsonObject mainObject, bool removeLineBreaks = true, bool addPadding = false);


	/**
	*Create a JSON Object from a JSON String.
	*@param JSON JSON String
	*@param removeAndSavePadding If it is JSON with padding, everything before { and after } is truncated and stored in the JSON object that is returned.
	*/
	UFUNCTION(BlueprintCallable, Category = "JSON")
		static FSJsonObject JsonStringToObject(FString JSON, bool removeAndSavePadding, bool& successful);

	UFUNCTION(BlueprintCallable, Category = "JSON")
		static void BreakJson_Object(const FSJsonObject JSONObject, TMap<FString, FSJsonObject>& ObjectMap, TMap<FString, bool>& BoolMap, TMap<FString, float>& NumberMap, TMap<FString, FString>& StringMap, TArray<FSJsonObject>& ObjectArray, TArray<bool>& BoolArray, TArray<float>& NumberArray, TArray<FString>& StringArray);

	UFUNCTION(BlueprintCallable, Category = "JSON")
		static void BreakMixedJSONArray(const int32 index, const FSJsonObject JSONObject, int32& lastIndex, EJsonDataType& type, FSJsonObject& Object, bool& Bool, float& Number, FString& String);

	UFUNCTION(BlueprintCallable, Category = "JSON")
		static struct FSJsonObject MakeMixedJSONArray(const FSJsonObject oldJSONMixedArray, EJsonDataType ChooseOneType, FSJsonObject Object, bool Bool, float Number, FString String);


	UFUNCTION(BlueprintCallable, Category = "JSON")
		static FSJsonObject VectorToJsonObject(FVector vector);

	UFUNCTION(BlueprintCallable, Category = "JSON")
		static FVector JsonObjectToVector(struct FSJsonObject JSONObject);

	UFUNCTION(BlueprintCallable, Category = "JSON")
		static FSJsonObject RotatorToJsonObject(FRotator rotator);

	UFUNCTION(BlueprintCallable, Category = "JSON")
		static FRotator JsonObjectToRotator(struct FSJsonObject JSONObject);

	UFUNCTION(BlueprintCallable, Category = "JSON")
		static FSJsonObject TransformToJsonObject(FTransform transform);

	UFUNCTION(BlueprintCallable, Category = "JSON")
		static FTransform JsonObjectToTransform(struct FSJsonObject JSONObject);
	
	UFUNCTION(BlueprintCallable, Category = "JSON")
		static void setStructJsonSettings(EJSONJsonStructMapSetting map);
	
	/**
	*Fills a struct with json data.
	*@param structID The StructID from "PrepareStructToFillItWithJson" is used to read the Struct from the cache. After the ID has been read from the cache, the struct is deleted from the cache.
	*@param json The Json Object with the appropriate data for the Struct
	*/
	UFUNCTION(BlueprintCallable, Category = "JSON")
		static void FillStructWithJson(FString structID, struct FSJsonObject json);


	//Thanks pedro_clericuzzi from the ue4 forum for figuring out how to read structures.

	/**
	*Caches the struct in order to fill it with JSON data in the next step. Must always be called before "Fill Struct With Json". 
	*@param Struct The struct to be filled with the json data.
	*/
	UFUNCTION(BlueprintCallable, Category = "JSON", CustomThunk, meta = (CustomStructureParam = "Struct"))
		static FString PrepareStructToFillItWithJson(UStruct* Struct) {
		return FString();
	};

	
	DECLARE_FUNCTION(execPrepareStructToFillItWithJson) {
		Stack.Step(Stack.Object, NULL);
		if (FProperty* StructProperty = CastField<FProperty>(Stack.MostRecentProperty)) {
			void* StructPtr = Stack.MostRecentPropertyAddress;
			P_FINISH;
			FString id = FGuid::NewGuid().ToString();
			FSFillStructWithJsonContainer fillStructWithJson;
			fillStructWithJson.structProperty = StructProperty;
			fillStructWithJson.structValue = StructPtr;
			prepareStructAuthoredNames(StructProperty);
			UJSONParserBPLibrary::getJSONParserBPLibrary()->storeStruct(id, fillStructWithJson);
			*(FString*)RESULT_PARAM = id;//parseStructToFSJsonObject(StructProperty, StructPtr);
		}
	}

	/**
	*Reads a struct and returns a JsonObject that can be processed further
	*@param Struct
	*/
	UFUNCTION(BlueprintCallable, Category = "JSON", CustomThunk, meta = (CustomStructureParam = "Struct"))
		static FSJsonObject StructToJsonObject(UStruct* Struct) {
		return FSJsonObject{};
	};

	DECLARE_FUNCTION(execStructToJsonObject){
		Stack.Step(Stack.Object, NULL);
		FProperty* StructProperty = CastField<FProperty>(Stack.MostRecentProperty);
		void* StructPtr = Stack.MostRecentPropertyAddress;
		P_FINISH;
		*(FSJsonObject*)RESULT_PARAM = parseStructToFSJsonObject(StructProperty, StructPtr);
	}
	

	void storeStruct(FString id, struct FSFillStructWithJsonContainer fillStructWithJson);
	FSFillStructWithJsonContainer* getStruct(FString id);
	void removeStruct(FString id);
	
	//File helper
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static void fileToBase64String(EJSONDirectoryType directoryType, FString filePath, FString& base64String, FString& fileName);
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static bool base64StringToFile(EJSONDirectoryType directoryType, FString base64String, FString filePath);
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static bool readStringFromFile(EJSONDirectoryType directoryType, FString filePath, FString& JSON_String);
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static void readBytesFromFile(EJSONDirectoryType directoryType, FString filePath, TArray<uint8>& bytes);
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static bool writeStringToFile(EJSONDirectoryType directoryType, FString JSON_String, FString filePath);
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static bool writeBytesToFile(EJSONDirectoryType directoryType, TArray<uint8> bytes, FString filePath);
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static bool deleteFile(EJSONDirectoryType directoryType, FString filePath);
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static bool createFile(EJSONDirectoryType directoryType, FString filePath);
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static bool fileExists(EJSONDirectoryType directoryType, FString filePath);
	UFUNCTION(BlueprintCallable, Category = "JSON|File")
		static bool dirExists(EJSONDirectoryType directoryType, FString filePath);

	//Async  tests
	////Delegates
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJSONParserReadStringFromFileEventDelegate, FString, jsonString, FString, optionalID);
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJSONParserRreadBytesFromFileEventDelegate, const TArray<uint8>&, bytes, FString, optionalID);
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJSONParserWriteStringToFileEventDelegate, bool, success, FString, optionalID);
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJSONParserWriteBytesToFileEventDelegate, bool, success, FString, optionalID);


	//UFUNCTION()
	//	void JSONParserReadStringFromFileEventDelegate(const FString jsonString, const FString optionalID);
	//UPROPERTY(BlueprintAssignable, Category = "JSON|File|Events|ReadStringFromFileAsync")
	//	FJSONParserReadStringFromFileEventDelegate onJSONParserReadStringFromFileEventDelegate;
	//UFUNCTION()
	//	void JSONParserRreadBytesFromFileEventDelegate(const TArray<uint8>& bytes, const FString optionalID);
	//UPROPERTY(BlueprintAssignable, Category = "JSON|File|Events|ReadBytesFromFileAsync")
	//	FJSONParserRreadBytesFromFileEventDelegate onJSONParserRreadBytesFromFileEventDelegate;
	//UFUNCTION()
	//	void JSONParserWriteStringToFileEventDelegate(const bool success, const FString optionalID);
	//UPROPERTY(BlueprintAssignable, Category = "JSON|File|Events|WriteStringToFileAsync")
	//	FJSONParserWriteStringToFileEventDelegate onJSONParserWriteStringToFileEventDelegate;
	//UFUNCTION()
	//	void JSONParserWriteBytesToFileEventDelegate(const bool success, const FString optionalID);
	//UPROPERTY(BlueprintAssignable, Category = "JSON|File|Events|WriteBytesToFileAsync")
	//	FJSONParserWriteBytesToFileEventDelegate onJSONParserWriteBytesToFileEventDelegate;

	//UFUNCTION(BlueprintCallable, Category = "JSON|File")
	//	void readStringFromFileAsync(EJSONDirectoryType directoryType, FString filePath, FString optionalID, bool switchToGameThread = true);
	//UFUNCTION(BlueprintCallable, Category = "JSON|File")
	//	void readBytesFromFileAsync(EJSONDirectoryType directoryType, FString filePath, FString optionalID, bool switchToGameThread = true);
	//UFUNCTION(BlueprintCallable, Category = "JSON|File")
	//	void writeStringToFileAsync(EJSONDirectoryType directoryType, FString JSON_String, FString filePath, FString optionalID, bool switchToGameThread = true);
	//UFUNCTION(BlueprintCallable, Category = "JSON|File")
	//	void writeBytesToFileAsync(EJSONDirectoryType directoryType, TArray<uint8> bytes, FString filePath, FString optionalID, bool switchToGameThread = true);

	UFUNCTION(BlueprintCallable, Category = "JSON|Compress")
		static void compressData(EJsonCompressFormat format, TArray<uint8> uncompressedData, TArray<uint8>& compressedData, int32& compressedDataSize, int32& uncompressedDataSize, bool& success);

	UFUNCTION(BlueprintCallable, Category = "JSON|Compress")
		static void uncompressData(EJsonCompressFormat format, int32 uncompressedDataSize,  TArray<uint8> compressedData, TArray<uint8>& uncompressedData, bool& success);

	UFUNCTION(BlueprintCallable, Category = "JSON|Compress")
		static TArray<uint8> stringToByteArray(FString string);

	UFUNCTION(BlueprintCallable, Category = "JSON|Compress")
		static FString byteArrayToString(TArray<uint8> byteArray);
	

private:
	EJSONJsonStructMapSetting mapSetting = EJSONJsonStructMapSetting::E_object;

	TMap<FString, struct FSFillStructWithJsonContainer> fillStructMap;

	static struct FSJsonObject generateFSJsonObjectWithParameter(TMap<FString, FSJsonObject> ObjectMap, TMap<FString, bool> BoolMap, TMap<FString, float> NumberMap, TMap<FString, FString> StringMap, TArray<FSJsonObject> ObjectArray, TArray<bool> BoolArray, TArray<float> NumberArray, TArray<FString> StringArray);
	static struct FSJsonObject generateFSJsonObject(struct FSJsonObject object);
	static struct FSJsonObject generateFSJsonObjectAndArray(FSJsonObject JSONObject, TArray< TSharedPtr<FJsonValue> > ue4Array);
	static struct FSJsonObject parseStructToFSJsonObject(FProperty* Property, void* ValuePtr);
	static void parseStructPropertyFSJsonObject(FProperty* Property, void* StructPtr, struct FSJsonObject& jsonObject, bool isArray = false);

	//template<typename FPropertyType, typename PropertyType>;
	static void fillStructToFSJsonObject(FProperty* Property, void* ValuePtr, TSharedPtr<FJsonObject> ue4Object = nullptr, TSharedPtr<FJsonValue> arrayValue = nullptr);
	static void prepareStructAuthoredNames(FProperty* Property);
	static FString authoredNameToFString(FString authoredName);
	static FString fStringToAuthoredName(FString name);
	TMap<FString, FString> authoredNames;

	//class FJSONParserThread* jsonThread = nullptr;
};