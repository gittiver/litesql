#ifndef DDX_H
#define DDX_H

#define TRANSFER_TEXT(textControl,stringVar,toWindow) \
  if (toWindow) { \
  (textControl)->SetValue(stringVar); \
  } else { \
   (stringVar)=(textControl)->GetValue();\
  }
  
#define TRANSFER_CHOICE(choiceCtrl,stringVar,toWindow) \
  if (toWindow) { \
  (choiceCtrl)->SetStringSelection(stringVar()); \
  } else { \
   stringVar((choiceCtrl)->GetStringSelection());\
  }

#define TRANSFER_BOOL(checkBoxCtrl,boolValue,toWindow) \
  if (toWindow) { \
  (checkBoxCtrl)->SetValue(boolValue()); \
  } else { \
   boolValue((checkBoxCtrl)->GetValue());\
  }

class TransferableData {

public:
  virtual bool TransferDataToWindow()
    { return TransferData(true); }

    virtual bool TransferDataFromWindow()
    { return TransferData(false); }

  protected:
    virtual bool TransferData(bool toWindow)=0;

};

#endif // #ifndef DDX_H
