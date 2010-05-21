#ifndef DDX_H
#define DDX_H

#define TRANSFER_TEXT(textControl,stringVar,toWindow) \
  if (toWindow) { \
  (textControl)->SetValue(stringVar); \
  } else { \
   (stringVar)= (textControl)->GetValue();\
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
