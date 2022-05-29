// 명령어				옵션			카피할 경로								카피 받을 경로

xcopy					/y				.\Engine\Headers\*.h					.\Reference\Headers\
xcopy					/y				.\Engine\Export\*.*						.\Reference\Headers\

xcopy					/y				.\Tool\ObjTool\include\*.h				.\Tool\ObjTool\headers\

xcopy					/y				.\Engine\System\code\*.h				.\Reference\Headers\
xcopy					/y				.\Engine\System\bin\System.lib			.\Reference\Libraries\
xcopy					/y				.\Engine\System\bin\System.dll			.\Client\bin\
xcopy					/y				.\Engine\System\bin\System.dll			.\Tool\ObjTool\bin\
xcopy					/y				.\Engine\System\bin\System.dll			.\Tool\MapTool\bin\


xcopy					/y				.\Engine\Utility\code\*.h				.\Reference\Headers\
xcopy					/y				.\Engine\Utility\bin\Utility.lib		.\Reference\Libraries\
xcopy					/y				.\Engine\Utility\bin\Utility.dll		.\Client\bin\
xcopy					/y				.\Engine\Utility\bin\Utility.dll		.\Tool\ObjTool\bin\
xcopy					/y				.\Engine\Utility\bin\Utility.dll		.\Tool\MapTool\bin\

xcopy					/y				.\Engine\Resources\code\*.h				.\Reference\Headers\
xcopy					/y				.\Engine\Resources\bin\Resources.lib	.\Reference\Libraries\
xcopy					/y				.\Engine\Resources\bin\Resources.dll	.\Client\bin\
xcopy					/y				.\Engine\Resources\bin\Resources.dll	.\Tool\ObjTool\bin\
xcopy					/y				.\Engine\Resources\bin\Resources.dll	.\Tool\MapTool\bin\