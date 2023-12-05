(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: MsgBox
 * File: MsgBox.st
 * Author: Josh
 * Created: July 23, 2013
 ********************************************************************
 * Implementation of program MsgBox
 ********************************************************************)

PROGRAM _INIT
		
	MessageBox_Init(20);
	
END_PROGRAM


PROGRAM _CYCLIC
	
	IF AddHigh THEN
		AddHigh:=	0;
		
		MessageBox_AddMsgTop(ADR('High Priority'),ADR('Cool'),ADR(OK),ADR('Cool'),ADR(Cancel));

	END_IF
	
	IF AddMsg THEN		
		AddMsg:=	0;

		MessageBox_AddMsg(ADR('Custom Prompt'),ADR('Custom OK'),ADR(OK),ADR('Custom Cancel'),ADR(Cancel));
		PromptBox_AddMsg(ADR('Custom Prompt with input'),ADR(InputString),ADR('Custom OK'),ADR(OK),ADR('Custom Cancel'),ADR(Cancel));
		MessageBox_AddMsg(POPUP_BUTTONS_SAVE,POPUP_BUTTONS_SAVE,ADR(OK),POPUP_BUTTONS_CANCEL,ADR(Cancel));
		MessageBox_AddMsg(ADR('Start a Timer?'),POPUP_BUTTONS_OK,ADR(TON_1.IN),0,0);
		MessageBox_AddMsg(ADR('Remove the next box?'),POPUP_BUTTONS_OK,ADR(removeBox),POPUP_BUTTONS_CANCEL,0);
		MessageID:=MessageBox_AddMsg(POPUP_BUTTONS_TIMER,POPUP_BUTTONS_OK,0,ADR('Go Away'),0);

	END_IF
		
	IF OK THEN
		OK:=	0;
	END_IF

	IF Cancel THEN
		Cancel:=	0;
		TON_1.IN:=	0;
	END_IF

	TON_1(PT:=T#5s);	
	IF TON_1.Q THEN TON_1.IN:=0;MessageBox_AddMsg(ADR('BANG'),0,0,0,0); END_IF
	brsftoa(TIME_TO_REAL(TON_1.PT-TON_1.ET)/1000.0,ADR(MyPrompt));
	MyPrompt:= CONCAT(MyPrompt,' seconds');

	IF removeBox OR TON_1.Q THEN
		TON_1.IN:=	0;		
		removeBox:= 0;
		IF MessageID <> 0 THEN
			MessageBox_RemoveMsg(MessageID);
			MessageID:=	0;
		END_IF
	END_IF
		
	DemoMsgBoxAck.IgnoreText:='Ignore';
//	DemoMsgBoxAck.IsError:
	DemoMsgBoxAck.Message:='E-Stop Active';
	DemoMsgBoxAck.OKText:='Ok';
//	DemoMsgBoxAck.Password:=;
	DemoMsgBoxAck.pErrCol:= ADR(gErrorCollector);
	DemoMsgBoxAck();

	MessageBox_HMI(MessageBox);	

		
END_PROGRAM
