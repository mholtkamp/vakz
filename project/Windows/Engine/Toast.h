#ifndef TOAST_H
#define TOAST_H

//## **********************************************************************
//## Toast
//## 
//## Adds an on-screen toast message that fades away after a few seconds.
//##
//## Input:
//##   pText - pointer to null terminated string.
//## **********************************************************************
void Toast(const char* pText);

//## **********************************************************************
//## RenderToast
//##
//## Called by engine for rendering toast on screen.
//## **********************************************************************
void RenderToast();


#endif