#ifndef MESSAGEVIEW_H
#define MESSAGEVIEW_H

#include <ColumnListView.h>
#include <Message.h>
#include <String.h>

class MessageView : public BColumnListView {
public:
	MessageView();
	void ShowMessageData(BMessage *message);

private:
	

};

#endif
