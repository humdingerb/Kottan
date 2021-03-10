/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#include "editview.h"

#include <TextControl.h>
#include <StringView.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <Spinner.h>
#include <FilePanel.h>

#include <iostream>


EditView::EditView(BMessage *data_message, type_code data_type, const char *data_label, int32 data_index)
	:
	BView("editview", B_SUPPORTS_LAYOUT),
	fDataMessage(data_message),
	fDataType(data_type),	
	fDataLabel(data_label),
	fDataIndex(data_index)
{

	fEditable=true;

	//create layout
	fMainLayout = new BGroupLayout(B_VERTICAL);
	SetLayout(fMainLayout);

	//construct needed controls for the different data types and add them to the layout
	setup_controls();

}


bool
EditView::IsEditable()
{

	return fEditable;

}


void 
EditView::setup_controls()
{

	switch(fDataType)
	{

		case B_BOOL_TYPE:
		{
		
			BPopUpMenu *bool_popup = new BPopUpMenu("boolpopup");
			bool_popup->AddItem(new BMenuItem("false", NULL));
			bool_popup->AddItem(new BMenuItem("true", NULL));
			bool_popup->ItemAt(0)->SetMarked(true);
			BMenuField *bool_select = new BMenuField("",bool_popup);
			fMainLayout->AddView(bool_select);
			break;
		}

		//all integer types get the same input field but need different range constraints
		case B_INT8_TYPE:
		case B_INT16_TYPE:
		case B_INT32_TYPE:
		case B_INT64_TYPE:
		case B_UINT8_TYPE:
		case B_UINT16_TYPE:
		case B_UINT32_TYPE:
		case B_UINT64_TYPE:
		{
			BSpinner *integer_spinner = new BSpinner("","",new BMessage);
			fMainLayout->AddView(integer_spinner);
		
			int32 range_min, range_max;  // B_UINT64 only gets 32bit ranges for now, 
										//	let´s hope no one finds out ;-)
		
			switch(fDataType)
			{			
				case B_INT8_TYPE:
					range_min=-128;
					range_max=127;
					break;
	
				case B_INT16_TYPE:
					range_min=-32768;
					range_max=32767;
					break;

				case B_INT32_TYPE:
				case B_INT64_TYPE:
					range_min=-2147483648;
					range_max=2147483647;
					break;
					
				case B_UINT8_TYPE:
					range_min=0;
					range_max=255;
					break;
		
				case B_UINT16_TYPE:
					range_min=0;
					range_max=65535;
					break;
		
				case B_UINT32_TYPE:
				case B_UINT64_TYPE:
					range_min=0;
					range_max=2147483647;
					break;
			}
			
			integer_spinner->SetRange(range_min, range_max);			
			break;
		}

		case B_RECT_TYPE:
		{	
			
			BRect data_rect;
			fDataMessage->FindRect(fDataLabel, fDataIndex, &data_rect);
			
			BString left_text;
			BString top_text;
			BString right_text;
			BString bottom_text;

			left_text << data_rect.left;
			top_text << data_rect.top;
			right_text << data_rect.right;
			bottom_text << data_rect.bottom;
			
			BTextControl *left_textctrl = new BTextControl("left:",left_text.String(), new BMessage());
			BTextControl *top_textctrl = new BTextControl("top:",top_text.String(), new BMessage());
			BTextControl *right_textctrl = new BTextControl("right:",right_text.String(), new BMessage());
			BTextControl *bottom_textctrl = new BTextControl("bottom:", bottom_text.String(), new BMessage());

			fMainLayout->AddView(left_textctrl);
			fMainLayout->AddView(top_textctrl);
			fMainLayout->AddView(right_textctrl);
			fMainLayout->AddView(bottom_textctrl);

			break;
		}

		case B_STRING_TYPE:
		{
			const char *data_string; 
			fDataMessage->FindString(fDataLabel, fDataIndex, &data_string);
		
			BTextControl *string_text = new BTextControl("",data_string,new BMessage());
			fMainLayout->AddView(string_text);
			break;
		}

		default:
		{
			BStringView *not_editable_text = new BStringView("","not editable");
			fMainLayout->AddView(not_editable_text);
			fEditable=false;
			break;
		}
	}

}
