#include "ItemStack.hpp"

ItemStack::ItemStack(jobject obj)
{
	CurrentObject = obj;
}

jobject ItemStack::GetCurrentObject()
{
	return CurrentObject;
}

jclass ItemStack::GetCurrentClass()
{
	if (CurrentObject == NULL) return NULL;
	if (CurrentClass == NULL) CurrentClass = JNIHelper::env->GetObjectClass(CurrentObject);

	return CurrentClass;
}

Item ItemStack::GetItem() // DONT WORK 
{
	if (GetCurrentClass() == NULL) return NULL;

	if (ItemInstance.GetCurrentClass() == NULL)
	{
		if (getItemFieldID == NULL)
		{
			getItemFieldID = JNIHelper::env->GetFieldID(GetCurrentClass(), "field_151002_e", "Lnet/minecraft/item/Item;");
			if (getItemFieldID == NULL) return NULL;
		}

		if (getItemObject == NULL)
		{
			getItemObject = JNIHelper::env->GetObjectField(GetCurrentObject(), getItemFieldID);
			if (getItemObject == NULL) return NULL;
		}

		ItemInstance = Item(getItemObject);
	}

	return ItemInstance;
}

jstring ItemStack::getUnlocalizedName()
{
	if (GetCurrentClass() == NULL) return NULL;

	if (getUnlocalizedNameString == NULL)
	{
		if (getUnlocalizedNameMethodID == NULL)
		{
			getUnlocalizedNameMethodID = JNIHelper::env->GetMethodID(GetCurrentClass(), "func_77977_a", "()Ljava/lang/String;");
			if (getUnlocalizedNameMethodID == NULL) return NULL;
		}

		if (getUnlocalizedNameObject == NULL)
		{
			getUnlocalizedNameObject = JNIHelper::env->CallObjectMethod(GetCurrentObject(), getUnlocalizedNameMethodID);
			if (getUnlocalizedNameObject == NULL) return NULL;
		}

		getUnlocalizedNameString = (jstring)getUnlocalizedNameObject;
	}

	return getUnlocalizedNameString;
}

jstring ItemStack::getDisplayName()
{
	if (GetCurrentClass() == NULL) return NULL;

	if (getDisplayNameString == NULL)
	{
		if (getDisplayNameMethodID == NULL)
		{
			getDisplayNameMethodID = JNIHelper::env->GetMethodID(GetCurrentClass(), "func_82833_r", "()Ljava/lang/String;");
			if (getDisplayNameMethodID == NULL) return NULL;
		}

		if (getDisplayNameObject == NULL)
		{
			getDisplayNameObject = JNIHelper::env->CallObjectMethod(GetCurrentObject(), getDisplayNameMethodID);
			if (getDisplayNameObject == NULL) return NULL;
		}

		getDisplayNameString = (jstring)getDisplayNameObject;
	}

	return getDisplayNameString;
}