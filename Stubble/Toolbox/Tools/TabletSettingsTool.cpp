#include "TabletSettingsTool.hpp"
#include <maya/M3dView.h>

#include "msgpack.h"
#include "wintab.h"
#define PACKETDATA	(PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
#define PACKETMODE	PK_BUTTONS
#include "pktdef.h"

typedef UINT ( API * WTINFOA ) ( UINT, UINT, LPVOID );
typedef HCTX ( API * WTOPENA )( HWND, LPLOGCONTEXTA, BOOL );
typedef BOOL ( API * WTCLOSE ) ( HCTX );
typedef int  ( API * WTPACKETSGET ) (HCTX, int, LPVOID);
typedef BOOL ( API * WTPACKET ) ( HCTX, UINT, LPVOID );

extern WTINFOA gpWTInfoA;
extern WTOPENA gpWTOpenA;
extern WTCLOSE gpWTClose;
extern WTPACKETSGET gpWTPacketsGet;
extern WTPACKET gpWTPacket;

HINSTANCE ghWintab = NULL;
WTINFOA gpWTInfoA = NULL;
WTOPENA gpWTOpenA = NULL;
WTCLOSE gpWTClose = NULL;
WTPACKET gpWTPacket = NULL;
WTPACKETSGET gpWTPacketsGet = NULL;

#define GETPROCADDRESS(type, func) gp##func = (type)GetProcAddress(ghWintab, #func);



namespace Stubble
{

namespace Toolbox
{

//----------------------------------------------------------------------------------------------------
// TabletSettingsToolCommand
//----------------------------------------------------------------------------------------------------

const MString TabletSettingsToolCommand::sCommandName( "StubbleTabletSettingsToolCommand" );

MPxContext* TabletSettingsToolCommand::makeObj()
{
	mCurrentObject = new TabletSettingsTool();
	return mCurrentObject;
}

void* TabletSettingsToolCommand::creator()
{
	return new TabletSettingsToolCommand;
}

TabletSettingsToolCommand::TabletSettingsToolCommand()
{
	mCurrentObject = NULL;
}

TabletSettingsToolCommand::~TabletSettingsToolCommand()
{
	delete mCurrentObject;
}

MStatus	TabletSettingsToolCommand::doEditFlags()
{
	MArgParser pars = parser();
  // TODO

	return MS::kSuccess;
}

MStatus	TabletSettingsToolCommand::doQueryFlags()
{
	MArgParser pars = parser();
	// TODO

	return MS::kSuccess;
}

MStatus	TabletSettingsToolCommand::appendSyntax()
{
	MSyntax syn = syntax();
  // TODO

	return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------
// TabletSettingsTool
//----------------------------------------------------------------------------------------------------

TabletSettingsTool::TabletSettingsTool()
{
	setTitleString( "Stubble tablet settings Tool" );
	mInitFlag = false;

  //initTablet();
}

//----------------------------------------------------------------------------------------------------
// WINTAB test
//----------------------------------------------------------------------------------------------------
void TabletSettingsTool::initTablet()
{
  ghWintab = LoadLibraryA(  "Wintab32.dll" );
  if ( !ghWintab )
  {
	  DWORD err = GetLastError();
	  std::cout << "TabletSettingsTool: could not load Wintab32.dll; error: " << err << std::endl;
  }
  else
  {
    M3dView view = M3dView::active3dView();
    HWND hwnd = view.applicationShell();
    LOGCONTEXT tabletcontext;
    HCTX tablethandle;

    GETPROCADDRESS( WTINFOA, WTInfoA );
    GETPROCADDRESS( WTOPENA, WTOpenA );
    GETPROCADDRESS( WTCLOSE, WTClose );
    GETPROCADDRESS( WTPACKET, WTPacket );
    GETPROCADDRESS( WTPACKETSGET, WTPacketsGet );
    
    std::cout << "TabletSettingsTool: tablet present = " << gpWTInfoA(0, 0, NULL) << std::endl;

    gpWTInfoA(WTI_DEFCONTEXT, 0, &tabletcontext);
    tabletcontext.lcPktData = PACKETDATA;

    tablethandle = gpWTOpenA(hwnd, &tabletcontext, TRUE);

    PACKET packet;
    while (1) 
    {
      if (gpWTPacketsGet(tablethandle, 1, &packet))
      {
        std::cout << "pkNormalPressure = " << packet.pkNormalPressure << std::endl;
      }
      Sleep(10);
    }
  }
}

TabletSettingsTool::~TabletSettingsTool()
{
}

void TabletSettingsTool::getClassName( MString &aName ) const
{ 
	aName = "StubbleHapticSettingsTool";
}

void TabletSettingsTool::toolOnSetup( MEvent &event )
{
	mInitFlag = true;
	mView = M3dView::active3dView();
}

void TabletSettingsTool::toolOffCleanup()
{
  // do not close haptic device here
}

} // namespace Toolbox

} // namespace Stubble
