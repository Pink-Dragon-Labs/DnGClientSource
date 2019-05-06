//
// table.cpp
//

#include <stdlib.h>
#include <stdio.h>

#include "sol.hpp"
#include "array.hpp"
#include "kernel.hpp"
#include "object.hpp"
#include "pmachine.hpp"
#include "windows.h"
#include "textid.hpp"

enum {
	TableInit,
	TableAdd,
	TableDelete,
	TableLookup,
	TableLookupObj,
	TableCount,
	TableSetListBox,
	TableGetOptionText,
};

typedef struct  
{
	int skill, level;
	char *name;
} item_t;

item_t gItemNames[] = {
	{17, 1, "Potion of Clumsiness"},
	{17, 1, "Potion of Dexterity"},
	{17, 1, "Potion of Pain"},
	{17, 1, "Potion of Poison"},
	{17, 1, "Purifying Elixir"},
	{17, 2, "Life Elixir"},
	{17, 2, "Potion of Acid Curse"},
	{17, 2, "Potion of Cold Curse"},
	{17, 2, "Potion of Fire Curse"},
	{17, 2, "Potion of Health"},
	{17, 2, "Potion of Lightning Curse"},
	{17, 2, "Potion of Poison Curse"},
	{17, 2, "Potion of Strength"},
	{17, 2, "Potion of Weakness"},
	{17, 3, "Potion of Acid Shield"},
	{17, 3, "Potion of Cold Shield"},
	{17, 3, "Potion of Cure Poison"},
	{17, 3, "Potion of Fire Shield"},
	{17, 3, "Potion of Lightning Shield"},
	{17, 3, "Potion of Poison Shield"},
	{17, 3, "Potion of See Invisible"},
	{17, 4, "Potion of Death"},
	{17, 4, "Potion of Greater Health"},
	{17, 4, "Potion of Invisibility"},
	{17, 5, "Potion of Greater Invisibility"},
	{17, 5, "Potion of Invulnerability"},
	{17, 5, "Potion of Regeneration"},
	{17, 5, "Potion of Shifting"},
	{18, 1, "Iron Axe"},
	{18, 1, "Iron Broad Sword"},
	{18, 1, "Iron Claw"},
	{18, 1, "Iron Dagger"},
	{18, 1, "Iron Long Sword"},
	{18, 1, "Iron Mace"},
	{18, 1, "Iron Short Sword"},
	{18, 1, "Iron Throwing Dagger"},
	{18, 1, "Iron Two-Handed Sword"},
	{18, 1, "Oaken Cudgel"},
	{18, 1, "Oaken Maul"},
	{18, 2, "Steel Axe"},
	{18, 2, "Steel Broad Sword"},
	{18, 2, "Steel Claw"},
	{18, 2, "Steel Club"},
	{18, 2, "Steel Dagger"},
	{18, 2, "Steel Long Sword"},
	{18, 2, "Steel Mace"},
	{18, 2, "Steel Maul"},
	{18, 2, "Steel Short Sword"},
	{18, 2, "Steel Throwing Dagger"},
	{18, 2, "Steel Two-Handed Sword"},
	{18, 2, "Tempered Steel Axe"},
	{18, 2, "Tempered Steel Broad Sword"},
	{18, 2, "Tempered Steel Claw"},
	{18, 2, "Tempered Steel Club"},
	{18, 2, "Tempered Steel Dagger"},
	{18, 2, "Tempered Steel Long Sword"},
	{18, 2, "Tempered Steel Mace"},
	{18, 2, "Tempered Steel Maul"},
	{18, 2, "Tempered Steel Short Sword"},
	{18, 2, "Tempered Steel Throwing Dagger"},
	{18, 2, "Tempered Steel Two-Handed Sword"},
	{18, 3, "Mythril Axe"},
	{18, 3, "Mythril Broad Sword"},
	{18, 3, "Mythril Claw"},
	{18, 3, "Mythril Club"},
	{18, 3, "Mythril Dagger"},
	{18, 3, "Mythril Long Sword"},
	{18, 3, "Mythril Mace"},
	{18, 3, "Mythril Maul"},
	{18, 3, "Mythril Short Sword"},
	{18, 3, "Mythril Throwing Dagger"},
	{18, 3, "Mythril Two-Handed Sword"},
	{18, 4, "Obsidianite Axe"},
	{18, 4, "Obsidianite Broad Sword"},
	{18, 4, "Obsidianite Claw"},
	{18, 4, "Obsidianite Club"},
	{18, 4, "Obsidianite Dagger"},
	{18, 4, "Obsidianite Long Sword"},
	{18, 4, "Obsidianite Mace"},
	{18, 4, "Obsidianite Maul"},
	{18, 4, "Obsidianite Short Sword"},
	{18, 4, "Obsidianite Throwing Dagger"},
	{18, 4, "Obsidianite Two-Handed Sword"},
	{18, 5, "Admantium Axe"},
	{18, 5, "Admantium Broad Sword"},
	{18, 5, "Admantium Claw"},
	{18, 5, "Admantium Club"},
	{18, 5, "Admantium Dagger"},
	{18, 5, "Admantium Long Sword"},
	{18, 5, "Admantium Mace"},
	{18, 5, "Admantium Maul"},
	{18, 5, "Admantium Short Sword"},
	{18, 5, "Admantium Throwing Dagger"},
	{18, 5, "Admantium Two-Handed Sword"},
	{19, 1, "Belt"},
	{19, 1, "Cowl"},
	{19, 1, "Doublet"},
	{19, 1, "High Boots"},
	{19, 1, "Hose"},
	{19, 1, "Iron Helmet"},
	{19, 1, "Iron Wristbands"},
	{19, 1, "Leather Armor"},
	{19, 1, "Leather Boots"},
	{19, 1, "Leather Cowl"},
	{19, 1, "Leather Low Boots"},
	{19, 1, "Leather Pants"},
	{19, 1, "Leather Skirt"},
	{19, 1, "Leather Tunic"},
	{19, 1, "Long Leather Skirt"},
	{19, 1, "Long Skirt"},
	{19, 1, "Low Boots"},
	{19, 1, "Robe"},
	{19, 1, "Shoes"},
	{19, 1, "Short Skirt"},
	{19, 1, "Troll Leather Boots"},
	{19, 1, "Troll Leather Cowl"},
	{19, 1, "Troll Leather Pants"},
	{19, 1, "Troll Leather Shirt"},
	{19, 1, "Troll Leather Skirt"},
	{19, 1, "Troll Leather Tunic"},
	{19, 1, "Troll Low Boots"},
	{19, 1, "Troll Short Skirt"},
	{19, 1, "Tunic"},
	{19, 1, "Wooden Round Shield"},
	{19, 1, "Wooden Tower Shield"},
	{19, 2, "Iron Chain Vest"},
	{19, 2, "Iron Cowl"},
	{19, 2, "Iron Greaves"},
	{19, 2, "Iron Leggings"},
	{19, 2, "Iron Plate Armor"},
	{19, 2, "Iron Round Shield"},
	{19, 2, "Iron Sollerets"},
	{19, 2, "Iron Tower Shield"},
	{19, 2, "Steel Chain Vest"},
	{19, 2, "Steel Cowl"},
	{19, 2, "Steel Greaves"},
	{19, 2, "Steel Helmet"},
	{19, 2, "Steel Leggings"},
	{19, 2, "Steel Plate Armor"},
	{19, 2, "Steel Round Shield"},
	{19, 2, "Steel Sollerets"},
	{19, 2, "Steel Tower Shield"},
	{19, 2, "Steel Wristbands"},
	{19, 3, "Mythril Chain Vest"},
	{19, 3, "Mythril Cowl"},
	{19, 3, "Mythril Greaves"},
	{19, 3, "Mythril Helmet"},
	{19, 3, "Mythril Leggings"},
	{19, 3, "Mythril Plate Armor"},
	{19, 3, "Mythril Round Shield"},
	{19, 3, "Mythril Sollerets"},
	{19, 3, "Mythril Tower Shield"},
	{19, 3, "Mythril Wristbands"},
	{19, 3, "Tempered Steel Chain Vest"},
	{19, 3, "Tempered Steel Cowl"},
	{19, 3, "Tempered Steel Greaves"},
	{19, 3, "Tempered Steel Helmet"},
	{19, 3, "Tempered Steel Leggings"},
	{19, 3, "Tempered Steel Plate Armor"},
	{19, 3, "Tempered Steel Round Shield"},
	{19, 3, "Tempered Steel Sollerets"},
	{19, 3, "Tempered Steel Tower Shield"},
	{19, 3, "Tempered Steel Wristbands"},
	{19, 4, "Obsidianite Chain Vest"},
	{19, 4, "Obsidianite Cowl"},
	{19, 4, "Obsidianite Greaves"},
	{19, 4, "Obsidianite Helmet"},
	{19, 4, "Obsidianite Leggings"},
	{19, 4, "Obsidianite Plate Armor"},
	{19, 4, "Obsidianite Round Shield"},
	{19, 4, "Obsidianite Sollerets"},
	{19, 4, "Obsidianite Tower Shield"},
	{19, 4, "Obsidianite Wristbands"},
	{19, 5, "Admantium Chain Vest"},
	{19, 5, "Admantium Cowl"},
	{19, 5, "Admantium Greaves"},
	{19, 5, "Admantium Helmet"},
	{19, 5, "Admantium Leggings"},
	{19, 5, "Admantium Plate Armor"},
	{19, 5, "Admantium Round Shield"},
	{19, 5, "Admantium Sollerets"},
	{19, 5, "Admantium Tower Shield"},
	{19, 5, "Admantium Wristbands"},
	{-1, -1, NULL}
};

struct TableNode {
	TableNode() : value(-1),object(0)
	{
	}

	uint  	value;
	SCIWord  object;
};
	
struct TablePtr {
	~TablePtr() 
	{
		delete table;
	};

	TableNode* table;
	uint  	  size;
} tables[10];


void
KTable(argList)
{
	int tIndex = arg(2);

	switch (arg(1)) {
		case TableInit:
			if (tables[tIndex].table == NULL) {
				tables[tIndex].size  = arg(3);
				tables[tIndex].table = New TableNode[arg(3)];
			}
			break;

		case TableAdd:
			if (tables[tIndex].table != NULL) {
				// get the LongInt SCI object ID
				ObjectID longID = (ObjectID)arg ( 3 );

				// validate the object
				if ( !longID.IsObject() )
					msgMgr->Fatal ( "Invalid object passed to KTable (0x%x)", longID );

				// get the 32 bit integer value from the low/hi word properties
				long objValue = (SCIUWord)longID.GetIndexedProperty ( longLowWord ) + ((SCIUWord)longID.GetIndexedProperty ( longHiWord ) << 16);

				for (int i = 0; i < tables[tIndex].size; ++i) {
					if (tables[tIndex].table[i].object == 0) {
						tables[tIndex].table[i].value  = objValue;
						tables[tIndex].table[i].object = arg(4);
						break;
					}
				}
			}
			break;

		case TableDelete:
			if (tables[tIndex].table != NULL) {
				// get the LongInt SCI object ID
				ObjectID longID = (ObjectID)arg ( 3 );

				// validate the object
				if ( !longID.IsObject() )
					msgMgr->Fatal ( "Invalid object passed to KTable (0x%x)", longID );

				// get the 32 bit integer value from the low/hi word properties
				long objValue = (SCIUWord)longID.GetIndexedProperty ( longLowWord ) + ((SCIUWord)longID.GetIndexedProperty ( longHiWord ) << 16);

				for (int i = 0; i < tables[tIndex].size; ++i) {
					if (tables[tIndex].table[i].value == objValue) {
						tables[tIndex].table[i].value = -1;
						tables[tIndex].table[i].object = 0;
						break;
					}
				}
			}
			break;

		case TableLookup:
			pm.acc = 0;
			if (tables[tIndex].table != NULL) {
				// get the LongInt SCI object ID
				ObjectID longID = (ObjectID)arg ( 3 );

				// validate the object
				if ( !longID.IsObject() )
					msgMgr->Fatal ( "Invalid object passed to KTable (0x%x)", longID );

				// get the 32 bit integer value from the low/hi word properties
				long objValue = (SCIUWord)longID.GetIndexedProperty ( longLowWord ) + ((SCIUWord)longID.GetIndexedProperty ( longHiWord ) << 16);

				for (int i = 0; i < tables[tIndex].size; ++i) {
					if (tables[tIndex].table[i].value == objValue) {
						pm.acc = tables[tIndex].table[i].object;
						break;
					}
				}
			}
			break;

		case TableLookupObj:
			pm.acc = 0;
			if (tables[tIndex].table != NULL) {
				int searchVal = arg(3);
				for (int i = 0; i < tables[tIndex].size; ++i) {
					if (tables[tIndex].table[i].object == searchVal) {
						pm.acc = tables[tIndex].table[i].value;
						break;
					}
				}
			}
			break;

		case TableCount: {
			pm.acc = 0;

			if (tables[tIndex].table != NULL) {
				for (int i = 0; i < tables[tIndex].size; ++i) {
					if (tables[tIndex].table[i].object != 0) {
						pm.acc++;
					}
				}
			}
		}

		break;

		case TableSetListBox: {
			int skill = arg(2);
			HWND hItem = (HWND)arg(3);

			int index = 0;

			while ( gItemNames[index].skill != -1 ) {
				if ( gItemNames[index].skill == skill ) {
					while ( gItemNames[index].skill == skill ) {
						char str[1024];
						sprintf ( str, "(Difficulty %d)\t%s", gItemNames[index].level, gItemNames[index].name );
						SendMessage ( hItem, LB_ADDSTRING, NULL, (long)str );
						index++;
					}

					return;
				}

				index++;
			}
		}

		break;

		case TableGetOptionText: {
			int skill = arg(2), option = arg(3);
			TextID textID = (TextID)arg(4);

			int index = 0, count = 0;

			while ( gItemNames[index].skill != -1 ) {
				if ( gItemNames[index].skill == skill ) {
					while ( gItemNames[index].skill == skill ) {
						if ( count == option ) {
							textID.Add ( gItemNames[index].name );
							return;
						}

						count++;
						index++;
					}

					return;
				}

				index++;
			}
		}

		break;
	}
}

