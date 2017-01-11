#pragma once
#include <vector>
#include <map>
#include <string>
using namespace std;

struct FName
{
	int index;
	int number;
};

struct TArray
{
	void*	AllocatorInstance;
	int		ArrayNum;
	int		ArrayMax;
};

struct UObject
{
	/**V Table**/
	void *vt;
	/** Flag **/
	int	ObjectFlags;
	/** Index into GObjectArray...very private. */
	int	InternalIndex;
	/** Class the object belongs to. */
	void*	ClassPrivate;
	/** Name of this object */
	FName	NamePrivate;
	/** Object this object resides in. */
	UObject*	OuterPrivate;
};

struct UField : UObject
{
	UField *Next;
};

struct UProperty : UField
{
	int			ArrayDim;
	int			ElementSize;
	uint64_t	PropertyFlags;
	int			RepIndex;
	FName		RepNotifyFunc;
	int			Unknow;
	int			Offset_Internal;
	/** In memory only: Linked list of properties from most-derived to base **/
	UProperty*	PropertyLinkNext;
	/** In memory only: Linked list of object reference properties from most-derived to base **/
	UProperty*  NextRef;
	/** In memory only: Linked list of properties requiring destruction. Note this does not include things that will be destroyed byt he native destructor **/
	UProperty*	DestructorLinkNext;
	/** In memory only: Linked list of properties requiring post constructor initialization.**/
	UProperty*	PostConstructLinkNext;
};

struct UBoolProperty : UProperty
{
	/** Size of the bitfield/bool property. Equal to ElementSize but used to check if the property has been properly initialized (0-8, where 0 means uninitialized). */
	char FieldSize;
	/** Offset from the memeber variable to the byte of the property (0-7). */
	char ByteOffset;
	/** Mask of the byte byte with the property value. */
	char ByteMask;
	/** Mask of the field with the property value. Either equal to ByteMask or 255 in case of 'bool' type. */
	char FieldMask;
};

struct UArrayProperty : UProperty
{
	UProperty* Property;
};

struct UStruct : UField
{
	UStruct*	SuperStruct;
	UField*		Children;
	int			PropertiesSize;
	int			MinAlignment;
	TArray		Script;
	/** In memory only: Linked list of properties from most-derived to base **/
	UProperty*	PropertyLink;
	/** In memory only: Linked list of object reference properties from most-derived to base **/
	UProperty*	RefLink;
	/** In memory only: Linked list of properties requiring destruction. Note this does not include things that will be destroyed byt he native destructor **/
	UProperty*	DestructorLink;
	/** In memory only: Linked list of properties requiring post constructor initialization.**/
	UProperty*	PostConstructLink;
	/** Array of object references embedded in script code. Mirrored for easy access by realtime garbage collection code */
	TArray		ScriptObjectReferences;
};


struct UScriptStruct : UStruct
{
	int Size;
	/** ALIGNOF() of the structure **/
	int Alignment;
};

struct UStructProperty : UProperty
{
	UScriptStruct* ScriptStruct;
};

struct FUObjectItem
{
	// Pointer to the allocated object
	UObject *Object;
	// Internal flags
	int Flags;
	// UObject Owner Cluster Index
	int ClusterIndex;
	// Weak Object Pointer Serial number associated with the object
	int SerialNumber;
};

void get_obj_name(int obj, char *name, int size);
int get_obj_type(int obj);
void get_type_inst(int type, vector<int> &vec_inst);
void get_super_class(int type, vector<int>& vec_super_class);
void get_fname_name(long long fname, char *name, int size);
void get_class_props(int type, vector<int> &vec_props);
int get_prop_offset(int prop);
int get_bool_prop_value(int prop, int addr);
void set_bool_prop_value(int prop, int addr, char value);
int get_struct_prop(int prop);
int get_array_prop(int prop);
int get_prop_array_dim(int prop);
int get_prop_elem_size(int prop);