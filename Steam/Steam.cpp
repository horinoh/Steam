// Steam.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Steam.h"

#ifdef USE_DX
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
DX* DXInst = nullptr;
#endif

#pragma region AddCode
Steam* SteamInst = nullptr;
#pragma endregion

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_STEAM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STEAM));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STEAM));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STEAM);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_CREATE:
		if (nullptr == SteamInst) {
			SteamInst = new Steam();
		}
		SetTimer(hWnd, NULL, 1000 / 60, nullptr);
#ifdef USE_DX
		if (nullptr == DXInst) {
			DXInst = new DX();
		}
		if (nullptr != DXInst) {
			try {
				DXInst->OnCreate(hWnd);
			}
			catch (std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}
#endif
		break;
	case WM_SIZE:
#ifdef USE_DX
		if (nullptr != DXInst) {
			DXInst->OnSize(hWnd, hInst);
		}
#endif
		break;
	case WM_TIMER:
		if (nullptr != SteamInst) {
			SteamInst->OnTimer(hWnd, hInst);
		}
		SetTimer(hWnd, NULL, 1000 / 60, nullptr);
		break;
	case WM_KEYDOWN:
		if (nullptr != SteamInst) {
			SteamInst->HandleInput(wParam);
		}
		if (VK_ESCAPE == wParam) {
			SendMessage(hWnd, WM_DESTROY, 0, 0);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
#ifdef USE_DX
			if (nullptr != DXInst) {
				DXInst->OnPaint(hWnd, hInst);
			}
#endif
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
#ifdef USE_DX
		if (nullptr != DXInst) {
			DXInst->OnDestroy(hWnd, hInst);
		}
		SAFE_DELETE(DXInst);
#endif
		SAFE_DELETE(SteamInst);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#pragma region AddCode
Steam::Steam()
{
	//!< �R���\�[��
	setlocale(LC_ALL, "");
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	SetConsoleTitle(TEXT("Steam"));
	freopen_s(&StdOut, "CON", "w", stdout);
	freopen_s(&StdErr, "CON", "w", stderr);

	//!< 0 �͖����l�A������ AppID �����s���ꂽ�炱�������������� steam_appid.txt �͍폜����
	const uint32 AppID = 0;
	if (!SteamAPI_RestartAppIfNecessary(AppID)) {
		if (SteamAPI_Init()) {
			std::cout << "SteamAPI_Init() [ OK ]" << std::endl;

			if (SteamClient()) {
				SteamClient()->SetWarningMessageHook([](int Severity, const char* Text) {
					if (Severity >= 1) {
						std::cout << Severity << " :  " << Text << " [ WARNING ]" << std::endl;
					}
				});
			}

			if (SteamUser()) {
				if (SteamUser()->BLoggedOn()) {
					std::cout << "SteamUser()->BLoggedOn() [ OK ]" << std::endl;
				}
			}

			if (SteamApps()) {
				const uint32 SpaceWarAppID = 480;
				if (SteamApps()->BIsAppInstalled(SpaceWarAppID)) {
					std::array<char, 256> Folder;
					if (SteamApps()->GetAppInstallDir(SpaceWarAppID, Folder.data(), static_cast<uint32>(Folder.size()))) {
						std::cout << SpaceWarAppID << " is installed in => " <<  Folder.data() << std::endl;
					}
					std::array<DepotId_t, 256> Depots;
					const auto DepoCount = SteamApps()->GetInstalledDepots(SpaceWarAppID, Depots.data(), static_cast<uint32>(Depots.size()));
					for (uint32 i = 0; i < DepoCount;++i) {
						std::cout << SpaceWarAppID << "'s depot => " << Depots[i] << std::endl;
					}
				}
			}

			StartController();

			Steam::PrintUsage();
			StartClient();
		}
	}
}
Steam::~Steam()
{
	SAFE_DELETE(mpGameClient);
	SAFE_DELETE(mpGameServer);

	SteamAPI_Shutdown();

	//!< �R���\�[��
	SAFE_FCLOSE(StdOut);
	SAFE_FCLOSE(StdErr);
	FreeConsole();
}

void Steam::OnTimer(HWND hWnd, HINSTANCE hInstance)
{
	SteamAPI_RunCallbacks();

	if (nullptr != mpGameServer) {
		mpGameServer->OnTimer(hWnd, hInst);
	}
	if (nullptr != mpGameClient) {
		mpGameClient->OnTimer(hWnd, hInst);
	}

	if (SteamController()) {
		ControllerHandle_t ControllerHandles[STEAM_CONTROLLER_MAX_COUNT];
		const auto Count = SteamController()->GetConnectedControllers(ControllerHandles);
		for (auto i = 0; i < Count; ++i) {
			const auto CH = ControllerHandles[i];

			//SteamController()->ActivateActionSet(CH, mMenuActionSet);
			SteamController()->ActivateActionSet(CH, mInGameActionSet);

			const auto MoveActionData = SteamController()->GetAnalogActionData(CH, mMoveAction);
			const auto FireActionData = SteamController()->GetDigitalActionData(CH, mFireAction);

			const auto MenuUpActionData = SteamController()->GetDigitalActionData(CH, mMenuUpAction);
			const auto MenuDownActionData = SteamController()->GetDigitalActionData(CH, mMenuDownAction);
			const auto MenuSelectActionData = SteamController()->GetDigitalActionData(CH, mMenuSelectAction);
			const auto MenuCancelActionData = SteamController()->GetDigitalActionData(CH, mMenuCancelAction);

			if (MoveActionData.bActive) {
				if (fabs(MoveActionData.x) > FLT_EPSILON || fabs(MoveActionData.y) > FLT_EPSILON) {
					std::cout << MoveActionData.x << ", " << MoveActionData.y << std::endl;
				}
			}
			if (FireActionData.bState) {
				std::cout << "Fire" << std::endl;
			}

			if (MenuUpActionData.bState) {
				std::cout << "Up" << std::endl;
			}
			if (MenuDownActionData.bState) {
				std::cout << "Down" << std::endl;
			}
			if (MenuSelectActionData.bState) {
				std::cout << "Select" << std::endl;
			}
			if (MenuCancelActionData.bState) {
				std::cout << "Cancel" << std::endl;
			}
		}
	}
}

void Steam::StartServer()
{
	if (nullptr == mpGameServer) {
		mpGameServer = new GameServer();
	}
}
void Steam::StartClient()
{
	if (nullptr == mpGameClient) {
		mpGameClient = new GameClient();
	}
}

void Steam::StartController()
{
	if (SteamController()) {
		/**
		@brief �R���g���[���̐ݒ�
		@note �ʏ�C���X�g�[���̏ꍇ�Ȃ� C:\Program Files(x86)\Steam\controller_config �� game_actions_480.vdf �Ƃ������O�ŃA�N�V�����t�@�C����z�u���Ă����B(controller_config�f�B���N�g���͖�����΍��)
		@note Steam �N���C�A���g���N�����A�E��̃R���g���[���̃A�C�R���� Big Picture ���[�h�� - �R���t�B�O - �R���g���[���ݒ� - ��]�̃R���g���[���Ƀ`�F�b�N������ - ��������
		@note ���C�u���� - Spacewar - �Q�[�����Ǘ� - �R���g���[���ݒ� �ŃL�[���A�T�C������
		*/
		SteamController()->Init();

		mInGameActionSet = SteamController()->GetActionSetHandle("InGameControls");
		mMenuActionSet = SteamController()->GetActionSetHandle("MenuControls");

		mMoveAction = SteamController()->GetAnalogActionHandle("Move");
		mFireAction = SteamController()->GetDigitalActionHandle("Fire");

		mMenuUpAction = SteamController()->GetDigitalActionHandle("Menu_Up");
		mMenuDownAction = SteamController()->GetDigitalActionHandle("Menu_Down");
		mMenuSelectAction = SteamController()->GetDigitalActionHandle("Menu_Select");
		mMenuCancelAction = SteamController()->GetDigitalActionHandle("Menu_Cancel");
	}

}

void Steam::PrintUsage()
{
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "u : Print Usage" << std::endl;
	std::cout << "l : Create Lobby" << std::endl;
	std::cout << "f : Find Lobby" << std::endl;
	std::cout << "[0-9] : Join Lobby" << std::endl;
	std::cout << "- : Leave Lobby" << std::endl;
	std::cout << "t: Lobby Status" << std::endl;
	std::cout << "r: Toggle READY" << std::endl;
	std::cout << "s: Start Server" << std::endl;
	std::cout << "m: Chat Message" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
}
void Steam::HandleInput(WPARAM wParam)
{
	//!< �L�[�R�[�h : https://msdn.microsoft.com/ja-jp/library/windows/desktop/dd375731(v=vs.85).aspx
	static const WPARAM KEY_0 = 0x30;
	static const WPARAM KEY_9 = 0x39;
	static const WPARAM KEY_F = 0x46;
	static const WPARAM KEY_L = 0x4c;
	static const WPARAM KEY_M = 0x4d;
	static const WPARAM KEY_R = 0x52;
	static const WPARAM KEY_S = 0x53;
	static const WPARAM KEY_T = 0x54;
	static const WPARAM KEY_U = 0x55;
	static const WPARAM KEY_MINUS = 0xbd;

	if (KEY_U == wParam) {
		PrintUsage();
	}
	const auto ServerInst = SteamInst->GetGameServer();
	const auto ClientInst = GetGameClient();
	if (nullptr == ServerInst) {
		//!< �Q�[���T�[�o���쐬
		if (KEY_S == wParam) {
			SteamInst->StartServer();

			//if (nullptr != ClientInst) {
			//	const auto LobbySteamID = ClientInst->GetEnteredLobbySteamID();
			//	if (LobbySteamID.IsValid()) {
			//		SteamMatchmaking()->SetLobbyData(LobbySteamID, "game_starting", "1"); // ���g�p
			//	}
			//}
		}
	}
	if (nullptr != ClientInst) {
		//!< ���r�[�쐬
		if (KEY_L == wParam) {
			ClientInst->CreateLobby();
		}
		//!< ���r�[����
		if (KEY_F == wParam) {
			ClientInst->FindLobby();
		}
		//!< ���r�[�Q��
		if (KEY_0 <= wParam && KEY_9 >= wParam) {
			ClientInst->JoinLobby(static_cast<uint32>(wParam - KEY_0));
		}
		//!< ���r�[���E
		if (KEY_MINUS == wParam) {
			ClientInst->LeaveLobby();
		}
		//!< READY ��Ԃ̃g�O��
		if (KEY_R == wParam) {
			ClientInst->ToggleReady();
		}
		//!< ���r�[��Ԃ̕\��
		if (KEY_T == wParam) {
			ClientInst->PrintLobbyStatus(ClientInst->GetEnteredLobbySteamID());
		}
		//!< ���r�[�Ƀ`���b�g���b�Z�[�W�𑗂�
		if (KEY_M == wParam) {
			if (ClientInst->GetEnteredLobbySteamID().IsValid()) {
				if (SteamMatchmaking()) {
					const auto Message = "Hello World";
					SteamMatchmaking()->SendLobbyChatMsg(ClientInst->GetEnteredLobbySteamID(), Message, static_cast<int>(strlen(Message) + 1));
				}
			}
		}
	}
}

/**
@brief �T�[�o
*/
GameServer::GameServer()
{
	//!< �|�[�g : https://support.steampowered.com/kb_article.php?ref=8571-GLVN-8711
	if (SteamGameServer_Init(0, 8766, 27015, 27016, eServerModeAuthenticationAndSecure, "1.0.0.0")) {
		if (SteamGameServer()) {
			SteamGameServer()->SetModDir("ModDir");

			SteamGameServer()->SetProduct("Product");
			SteamGameServer()->SetGameDescription("Game Description");

			//SteamGameServer()->SetSpectatorPort( ... );
			//SteamGameServer()->SetSpectatorServerName( ... );

			SteamGameServer()->LogOnAnonymous();

			SteamGameServer()->EnableHeartbeats(true);

			const auto ServerSteamID = SteamGameServer()->GetSteamID();
			std::cout << "ServerSteamID = " << ServerSteamID.ConvertToUint64() << std::endl;
		}
	}

	//SendUpdatedServerDetailsToSteam();
}
GameServer::~GameServer()
{
	if (SteamGameServer()) {
		SteamGameServer()->EnableHeartbeats(false);

		//!< #MY_TODO �I�����N���C�A���g�֒m�点��
		//SteamGameServerNetworking()->SendP2PPacket();

		SteamGameServer()->LogOff();
	}

	SteamGameServer_Shutdown();
}

void GameServer::OnTimer(HWND hWnd, HINSTANCE hInstance)
{
	switch (mState)
	{
	case GameServer::EServerState::None:
		break;
	default:
		break;
	}

	ReceiveData();
}
void GameServer::ReceiveData()
{
	if (SteamGameServerNetworking()) {
		const auto ServerSteamID = SteamGameServer()->GetSteamID();
		const auto ServerSteamIDU64 = ServerSteamID.ConvertToUint64();

		uint32 Size = 0;
		while (SteamGameServerNetworking()->IsP2PPacketAvailable(&Size)) {
			if (Size) {
				auto Buffer = new char[Size];
				CSteamID RemoteSteamID;
				if (SteamGameServerNetworking()->ReadP2PPacket(Buffer, Size, &Size, &RemoteSteamID)) {
					std::cout << "Received from : " << RemoteSteamID.ConvertToUint64() << ", Size = " << Size << std::endl;

					if (SteamGameServer()) {
						//!< ���������� OnValidateAuthTicketResponse() ���R�[���o�b�N�����
						//if (k_EBeginAuthSessionResultOK == SteamGameServer()->BeginAuthSession()) {
						//}
					}
				}
				delete[] Buffer;
			}
		}
	}
}

/**
@brief SteamGameServer()->BeginAuthSession() �̃R�[���o�b�N�� ValidateAuthTicketResponse_t �Ŏ󂯂�
*/
void GameServer::OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t* pCallback)
{
}

/**
@brief  �̃R�[���o�b�N�� SteamServersConnected_t �Ŏ󂯂�
*/
void GameServer::OnSteamServersConnected(SteamServersConnected_t* pCallback)
{
	//m_bConnectedToSteam = true;
}
/**
@brief  �̃R�[���o�b�N�� SteamServerConnectFailure_t �Ŏ󂯂�
*/
void GameServer::OnSteamServerConnectFailure(SteamServerConnectFailure_t* pCallback)
{
}
/**
@brief  �̃R�[���o�b�N�� SteamServersDisconnected_t �Ŏ󂯂�
*/
void GameServer::OnSteamServersDisconnected(SteamServersDisconnected_t* pCallback)
{
}

/**
@brief �N���C�A���g
*/
GameClient::GameClient()
{
	if (SteamUserStats()) {
		//!< �X�^�b�c(����)�̃��N�G�X�g
		if (!SteamUserStats()->RequestCurrentStats()) {
			std::cout << "RequestCurrentStats() failed" << std::endl;
		}

		//!< ���[�_�{�[�h
		UploadLeaderboard("MyLeaderboard");
		//DownloadLeaderboard("MyLeaderboard");
		
		//!< DLC
		QueryDLC();

		//!< UGC(User Generated Content)  �̃N�G��
		QueryUGC();

		//!< UGC(User Generated Content) �̍쐬
		//CreateUGC();

		//!< RemoteStorage �̃N�G��
		QueryRemoteStoage();

		//!< �t�����h���̎擾 #MY_TODO $5�������O���ăt�����h�֘A���`�F�b�N����
		GetFriendInformation();
	}
}
GameClient::~GameClient()
{
	StoreStats();
	//ResetStats();

	if (SteamMatchmaking()) {
		if (k_HAuthTicketInvalid != mAuthTicket) {
			if (SteamUser()) {
				SteamUser()->CancelAuthTicket(mAuthTicket);
			}
			mAuthTicket = k_HAuthTicketInvalid;
		}

		LeaveLobby();
	}
}

void GameClient::OnTimer(HWND hWnd, HINSTANCE hInstance)
{
	switch (mState)
	{
	default:
	case EClientState::None:
		break;
	case EClientState::InLobby:
		if (mEnteredLobbySteamID.IsValid()) {
			if (false/*�T�[�o���ڑ����ꂽ*/) {
				if (SteamMatchmaking()) {
					const auto ServerSteamID = SteamGameServer()->GetSteamID();

					//!< �T�[�o�̏������ł����̂Őڑ�����
					SteamMatchmaking()->SetLobbyGameServer(mEnteredLobbySteamID, 0, 0, ServerSteamID);
					
					//!< ���r�[�͑ޏo����
					LeaveLobby();

					//!< �T�[�o�փN���C�A���g���ڑ��ɓ���������`����
					const uint8 Data = 1;
					SteamNetworking()->SendP2PPacket(ServerSteamID, &Data, sizeof(Data), k_EP2PSendUnreliable);
					//SteamMatchmakingServers()->PingServer(unIP, unPort, ISteamMatchmakingPingResponse);
				}
			}
		}
		mState = EClientState::Connecting;
		break;
	case EClientState::CreatingLobby:
		break;
	case EClientState::FindingLobby:
		break;
	case EClientState::JoiningLobby:
		break;
	case EClientState::Connecting:
		//!< �^�C���E�A�E�g�����炷�鏈��
		//SteamUser()->TerminateGameConnection(m_unServerIP, m_usServerPort);
		break;
	}

	ReceiveData();
}
void GameClient::ReceiveData()
{
	if (SteamNetworking()) {
		uint32 Size = 0;
		while (SteamNetworking()->IsP2PPacketAvailable(&Size)) {
			if (Size) {
				auto Buffer = new char[Size];
				CSteamID RemoteSteamID;
				if (SteamNetworking()->ReadP2PPacket(Buffer, Size, &Size, &RemoteSteamID)) {
					std::cout << "Received " << Size << std::endl;
				}
				delete[] Buffer;
			}
		}
	}
}

/**
@brief �̃R�[���o�b�N�� SteamServersConnected_t �Ŏ󂯂�
*/
void GameClient::OnSteamServersConnected(SteamServersConnected_t* pCallback)
{
}
/**
@brief �̃R�[���o�b�N�� SteamServersDisconnected_t �Ŏ󂯂�
*/
void GameClient::OnSteamServersDisconnected(SteamServersDisconnected_t* pCallback)
{
}
/**
@brief �̃R�[���o�b�N�� SteamServerConnectFailure_t �Ŏ󂯂�
*/
void GameClient::OnSteamServerConnectFailure(SteamServerConnectFailure_t* pCallback)
{
}

/**
@brief SteamMatchmaking()->CreateLobby() �̃R�[���o�b�N�� LobbyCreated_t �Ŏ󂯂�
*/
void GameClient::OnLobbyCreated(LobbyCreated_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure) {
		//!< ���Ƀ��r�[�ɓ����Ă���Ȃ甲����
		LeaveLobby();

		mEnteredLobbySteamID = pCallback->m_ulSteamIDLobby;
		if (SteamUser() && SteamMatchmaking()) {
			mState = EClientState::InLobby;

			//!< �����t�B���^�p�ɁA�Q�[������ݒ肵�Ă��� ["game"] = "SpaceWar"
			SteamMatchmaking()->SetLobbyData(mEnteredLobbySteamID, "game", "SpaceWar");

			//SteamMatchmaking()->SetLobbyData(mEnteredLobbySteamID, "name", (std::to_string(SteamUser()->GetSteamID().ConvertToUint64()) + "'s LOBBY").c_str());
			SteamMatchmaking()->SetLobbyData(mEnteredLobbySteamID, "name", (std::string(SteamFriends()->GetPersonaName()) + "'s LOBBY").c_str());

			std::cout << "Created : " << mEnteredLobbySteamID.ConvertToUint64() << " (" << SteamMatchmaking()->GetLobbyData(mEnteredLobbySteamID, "name")  << ")" << std::endl;

			PrintLobbyStatus(mEnteredLobbySteamID);
		}
	}
}
/**
@brief SteamMatchmaking()->RequestLobbyList() �̃R�[���o�b�N�� LobbyMatchList_t �Ŏ󂯂�
*/
void GameClient::OnLobbyMatchListed(LobbyMatchList_t* pCallback, bool bIOFailure)
{
	mState = mEnteredLobbySteamID.IsValid() ? EClientState::InLobby: EClientState::None;

	std::cout << "---- Found Lobby Match List -------------------" << std::endl;
	mFoundLoobySteamID.clear();
	if (!bIOFailure) {
		if (SteamMatchmaking() && SteamUser()) {
			const auto MySteamID = SteamUser()->GetSteamID();

			const auto Count = pCallback->m_nLobbiesMatching;
			for (uint32 i = 0; i < Count; ++i) {
				const auto LobbySteamID = SteamMatchmaking()->GetLobbyByIndex(i);
				
				//!< �����̍쐬�������r�[�͑I�����Ɋ܂߂Ȃ�
				if (MySteamID != SteamMatchmaking()->GetLobbyOwner(LobbySteamID)) {
					std::cout << "[" << mFoundLoobySteamID.size() << "] : " << LobbySteamID.ConvertToUint64() << " (" << SteamMatchmaking()->GetLobbyData(LobbySteamID, "name") << ")" << std::endl;
					mFoundLoobySteamID.push_back(LobbySteamID);
				}
				else {
					std::cout << "[-] : " << LobbySteamID.ConvertToUint64() << " (" << SteamMatchmaking()->GetLobbyData(LobbySteamID, "name") << ")" << std::endl;
				}
			}
		}
	}
	std::cout << "-----------------------------------------------" << std::endl;
}
/**
@brief SteamMatchmaking()->JoinLobby() �̃R�[���o�b�N�� LobbyEnter_t �Ŏ󂯂�
*/
void GameClient::OnLobbyEntered(LobbyEnter_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure) {
		//!< ���Ƀ��r�[�ɓ����Ă���Ȃ甲����
		LeaveLobby();

		if (SteamMatchmaking()) {
			mState = EClientState::InLobby;

			mEnteredLobbySteamID = pCallback->m_ulSteamIDLobby;

			std::cout << "Entered : " << mEnteredLobbySteamID.ConvertToUint64() << " (" << SteamMatchmaking()->GetLobbyData(mEnteredLobbySteamID, "name") << ")" << std::endl;

			PrintLobbyStatus(mEnteredLobbySteamID);
		}
	}
}

/**
@brief SteamMatchmaking()->SetLobbyGameServer() �̃R�[���o�b�N�� LobbyGameCreated_t �Ŏ󂯂�
*/
void GameClient::OnLobbyGameCreated(LobbyGameCreated_t* pCallback)
{
	std::cout << "m_ulSteamIDLobby : " << pCallback->m_ulSteamIDLobby << std::endl;
	std::cout << "m_ulSteamIDGameServer : " << pCallback->m_ulSteamIDGameServer << std::endl;
	std::cout << "m_unIP : " << pCallback->m_unIP << std::endl;
	std::cout << "m_usPort : " << pCallback->m_usPort << std::endl;

	if (CSteamID(pCallback->m_ulSteamIDLobby).IsValid()) {
		if (SteamMatchmaking()) {
			SteamMatchmaking()->LeaveLobby(pCallback->m_ulSteamIDLobby);
		}
	}

	if (SteamNetworking()) {
		//if (SteamNetworking()->SendP2PPacket(CSteamID(pCallback->m_ulSteamIDGameServer), pData, nSizeOfData, k_EP2PSendUnreliable)) {
		//}
	}
}

/**
@brief SteamMatchmaking()->SendLobbyChatMsg() �̃R�[���o�b�N�� LobbyChatMsg_t �Ŏ󂯂�
*/
void GameClient::OnLobbyChatMsg(LobbyChatMsg_t* pCallback)
{
	if (SteamMatchmaking()) {
		char Buffer[BUFSIZ];
		const auto WrittenBytes = SteamMatchmaking()->GetLobbyChatEntry(pCallback->m_ulSteamIDLobby, pCallback->m_iChatID, nullptr, Buffer, sizeof(Buffer), nullptr);
		std::cout << "[" << pCallback->m_ulSteamIDUser << "] : " << Buffer << std::endl;
	}
}

void GameClient::CreateLobby()
{
	if (SteamMatchmaking()) {
		if (!mOnLobbyCreated.IsActive()) {
			const auto Handle = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 4);
			if (k_uAPICallInvalid != Handle) {
				mOnLobbyCreated.Set(Handle, this, &GameClient::OnLobbyCreated);
				mState = EClientState::CreatingLobby;
			}
		}
	}
}
void GameClient::FindLobby()
{
	if (SteamMatchmaking()) {
		//!< ["game"] = "SpaceWar" �̂��̂����t�B���^�����O����ꍇ
		//SteamMatchmaking()->AddRequestLobbyListStringFilter("game", "SpaceWar", k_ELobbyComparisonEqual);

		//!< (�����ł�) ������̂� 10 �܂łɂ��Ă���
		SteamMatchmaking()->AddRequestLobbyListResultCountFilter(10);

		const auto Handle = SteamMatchmaking()->RequestLobbyList();
		if (k_uAPICallInvalid != Handle) {
			mOnLobbyMatchListed.Set(Handle, this, &GameClient::OnLobbyMatchListed);
			mState = EClientState::FindingLobby;
		}
	}
}
void GameClient::JoinLobby(const uint32 Index)
{
	if (SteamMatchmaking()) {
		if (Index < mFoundLoobySteamID.size()) {
			const auto SteamID = mFoundLoobySteamID[Index];
			if (SteamID.IsValid()) {
				const auto Handle = SteamMatchmaking()->JoinLobby(SteamID);
				mOnLobbyEntered.Set(Handle, this, &GameClient::OnLobbyEntered);
				mState = EClientState::JoiningLobby;
			}
		}
	}
}

void GameClient::LeaveLobby()
{
	if (SteamMatchmaking()) {
		if (mEnteredLobbySteamID.IsValid()) {
			SteamMatchmaking()->LeaveLobby(mEnteredLobbySteamID);

			std::cout << "Leaved : " << mEnteredLobbySteamID.ConvertToUint64() << " (" << SteamMatchmaking()->GetLobbyData(mEnteredLobbySteamID, "name") << ")" << std::endl;

			mEnteredLobbySteamID = CSteamID();
		}
	}
}

void GameClient::PrintLobbyStatus(const CSteamID LobbySteamID)
{
	if (SteamMatchmaking()) {
		if (LobbySteamID.IsValid()) {
			std::cout << "---- " << SteamMatchmaking()->GetLobbyData(LobbySteamID, "name") << " ----------------" << std::endl;
			const auto LobbyOwnerSteamID = SteamMatchmaking()->GetLobbyOwner(LobbySteamID);

			const auto MemberCount = SteamMatchmaking()->GetNumLobbyMembers(LobbySteamID);
			for (auto i = 0; i < MemberCount; ++i) {
				const auto MemberSteamID = SteamMatchmaking()->GetLobbyMemberByIndex(LobbySteamID, i);

				const auto ReadyState = std::atoi(SteamMatchmaking()->GetLobbyMemberData(LobbySteamID, MemberSteamID, "ready"));
				const auto IsOwner = (MemberSteamID == LobbyOwnerSteamID);
				std::cout << (IsOwner ? "[Owner] " : "") << MemberSteamID.ConvertToUint64() << (1 == ReadyState ? " (READY) " : "") << std::endl;
			}

			const auto DataCount = SteamMatchmaking()->GetLobbyDataCount(LobbySteamID);
			for (auto i = 0; i < DataCount; ++i) {
				char Key[BUFSIZ], Value[BUFSIZ];
				if (SteamMatchmaking()->GetLobbyDataByIndex(LobbySteamID, i, Key, sizeof(Key), Value, sizeof(Value))) {
					std::cout << "\t[" << Key << "] = " << Value << std::endl;
				}
			}
			std::cout << "-----------------------------------------------" << std::endl;
		}
	}
}
void GameClient::ToggleReady()
{
	if (SteamMatchmaking()) {
		if (SteamUser()) {
			const auto LobbySteamID = mEnteredLobbySteamID;
			const auto MySteamID = SteamUser()->GetSteamID();

			if (LobbySteamID.IsValid() && MySteamID.IsValid()) {
				const auto ReadyState = !std::atoi(SteamMatchmaking()->GetLobbyMemberData(LobbySteamID, MySteamID, "ready"));
				SteamMatchmaking()->SetLobbyMemberData(LobbySteamID, "ready", std::to_string(ReadyState).c_str());

				PrintLobbyStatus(LobbySteamID);
			}
		}
	}
}

/**
@brief Spacewar �ł͈ȉ��̃X�^�b�c�A���т����݂���

	���[�U�X�^�b�c
	"NumGames", "NumWins", "NumLosses", "FeetTraveled", "AverageSpeed", "Unused2", "MaxFeetTraveled"

	����
	"ACH_WIN_ONE_GAME", "ACH_WIN_100_GAMES", "ACH_TRAVEL_FAR_ACCUM", "ACH_TRAVEL_FAR_SINGLE", "NEW_ACHIEVEMENT_0_4",

@note �����Ŋm�F�ł��� https://steamdb.info/app/480/stats/
*/
void GameClient::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	if (k_EResultOK == pCallback->m_eResult) {
		if (SteamUserStats()) {
			std::cout << "UserStatsReceived. GameID = " << pCallback->m_nGameID << ", UserName = " << SteamFriends()->GetFriendPersonaName(pCallback->m_steamIDUser) << std::endl;

			//!< �X�^�b�c
			std::cout << "Stats" << std::endl;
			if (SteamUserStats()->GetStat("NumGames", &mNumGames)) {
				std::cout << "\tNumGames = " << mNumGames << std::endl;
			}
			if (SteamUserStats()->GetStat("NumWins", &mNumWins)) {
				std::cout << "\tNumWins = " << mNumWins << std::endl;
			}
			if (SteamUserStats()->GetStat("NumLosses", &mNumLosses)) {
				std::cout << "\tNumLosses = " << mNumLosses << std::endl;
			}

			if (SteamUserStats()->GetStat("FeetTraveled", &mFeetTraveled)) {
				std::cout << "\tFeetTraveled  = " << mFeetTraveled << std::endl;
			}
			if (SteamUserStats()->GetStat("MaxFeetTraveled", &mMaxFeetTraveled)) {
				std::cout << "\tMaxFeetTraveled = " << mMaxFeetTraveled << std::endl;
			}

			float AverageSpeed;
			if (SteamUserStats()->GetStat("AverageSpeed", &AverageSpeed)) {
				std::cout << "\tAverageSpeed = " << AverageSpeed << std::endl;
			}

			//!< ����
			std::cout << "Achievements" << std::endl;
			bool bUnlocked;
			const auto Count = SteamUserStats()->GetNumAchievements();
			for (uint32 i = 0; i < Count; ++i) {
				const auto Name = SteamUserStats()->GetAchievementName(i);
				if (SteamUserStats()->GetAchievement(Name, &bUnlocked)) {
					std::cout << "\t" << Name << " : " << (bUnlocked ? "Unlocked" : "Locked") << std::endl;
					std::cout << "\t\t" << SteamUserStats()->GetAchievementDisplayAttribute(Name, "name") << std::endl;
					std::cout << "\t\t" << SteamUserStats()->GetAchievementDisplayAttribute(Name, "desc") << std::endl;

					//!< �A�C�R���f�[�^�̎擾
					const auto Icon = SteamUserStats()->GetAchievementIcon(Name);
					if (Icon && SteamUtils()) {
						uint32 Width, Height;
						if (SteamUtils()->GetImageSize(Icon, &Width, &Height)) {
							const auto Size = Width * Height * 4;
							if (Size > 0) {
								auto RGBA = new uint8[Size];
								if (SteamUtils()->GetImageRGBA(Icon, RGBA, Size)) {
								}
								delete[] RGBA;
							}
						}
					}

					//uint32 UnlockTime;
					//if (SteamUserStats()->GetAchievementAndUnlockTime(Name, &bUnlocked, &UnlockTime)) {
					//}
				}
			}
		}
	}
}
void GameClient::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	if (k_EResultOK == pCallback->m_eResult) {
		std::cout << "UserStatsStored. GameID = " << pCallback->m_nGameID << std::endl;
	}
}
void GameClient::OnUserAchievementStored(UserAchievementStored_t* pCallback)
{
	std::cout << "UserAchievementStored. GameID = " << pCallback->m_nGameID << std::endl;
	std::cout << "\t" << pCallback->m_rgchAchievementName << " is unlocked" << std::endl;
	std::cout << "\t" << "Progress = " << pCallback->m_nCurProgress << " / " << pCallback->m_nMaxProgress << std::endl;
	//std::cout << pCallback->m_bGroupAchievement << std::endl; //!< �O���[�v���т��ǂ���
}
void GameClient::StoreStats()
{
	//!< (�����ł�) �����_���Ɍ��ʂ��쐬

	SteamUserStats()->SetStat("NumGames", ++mNumGames);
	std::random_device rd;
	if (rd() % 2) {
		SteamUserStats()->SetStat("NumWins", ++mNumWins);
	}
	else {
		SteamUserStats()->SetStat("NumLosses", ++mNumLosses);
	}

	const auto Duration = 60.0f;
	const auto Feet = static_cast<float>(rd() % 501);
	SteamUserStats()->SetStat("FeetTraveled", (mFeetTraveled += Feet));
	if (Feet > mMaxFeetTraveled) {
		SteamUserStats()->SetStat("MaxFeetTraveled", (mMaxFeetTraveled = Feet));
	}
	SteamUserStats()->UpdateAvgRateStat("AverageSpeed", Feet, Duration);

	if (mNumWins >= 100) {
		if (SteamUserStats()->SetAchievement("ACH_WIN_100_GAMES")) {}
	}
	else if (mNumWins > 0) {
		if (SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME")) {}
	}
	else {
		if (SteamUserStats()->IndicateAchievementProgress("ACH_WIN_100_GAMES", mNumWins, 100)) {}
	}
	if (mFeetTraveled > 5280) {
		//!< 1�}�C�� = 5280�t�B�[�g
		if (SteamUserStats()->SetAchievement("ACH_TRAVEL_FAR_ACCUM")) {}
	}
	if (Feet > 500) {
		if (SteamUserStats()->SetAchievement("ACH_TRAVEL_FAR_SINGLE")) {}
	}

	SteamUserStats()->StoreStats();
}

void GameClient::ResetStats()
{
	if (SteamUserStats()) {
		SteamUserStats()->ResetAllStats(true);
		SteamUserStats()->StoreStats();
	}
}

void GameClient::OnLeaderboardFindResultAndUpload(LeaderboardFindResult_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && pCallback->m_bLeaderboardFound) {
		if (SteamUserStats()) {
			std::cout << SteamUserStats()->GetLeaderboardName(pCallback->m_hSteamLeaderboard) << std::endl;

			//!< �A�b�v���[�h�J�n
			std::random_device rd;
			const int32 Score = rd() % 101;
			//!< �A�b�v���[�h���� details ���w�肷��ƁA�_�E�����[�h���Ɏ擾�ł��� (int32 * k_cLeaderboardDetailsMax(=64))
			//!< UGC�ɂ���قǂł��Ȃ��f�[�^��t�������ꍇ
			const std::vector<int32> details = { 0, 1, 2, 3 };
			const auto Handle = SteamUserStats()->UploadLeaderboardScore(pCallback->m_hSteamLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, Score, details.data(), static_cast<int>(details.size()));
			if (k_uAPICallInvalid != Handle) {
				mLeaderboardScoreUploaded.Set(Handle, this, &GameClient::OnLeaderboardScoreUploaded);
			}
		}
	}
}

void GameClient::OnLeaderboardFindResultAndDownload(LeaderboardFindResult_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && pCallback->m_bLeaderboardFound) {
		if (SteamUserStats()) {
			std::cout << SteamUserStats()->GetLeaderboardName(pCallback->m_hSteamLeaderboard) << std::endl;

#if 0
			//!< �w��͈�(����)�̃��[�_�[�{�[�h���擾
			const auto Handle = SteamUserStats()->DownloadLeaderboardEntries(pCallback->m_hSteamLeaderboard, ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal, 0, 10);
#else
			//!< ���g�̃��[�_�[�{�[�h���擾
			std::vector<CSteamID> SteamIDs = { SteamUser()->GetSteamID(), /*...*/ };
			const auto Handle = SteamUserStats()->DownloadLeaderboardEntriesForUsers(pCallback->m_hSteamLeaderboard, SteamIDs.data(), static_cast<int>(SteamIDs.size()));
#endif
			if (k_uAPICallInvalid != Handle) {
				mLeaderboardScoresDownloaded.Set(Handle, this, &GameClient::OnLeaderboardScoresDownloaded);
			}
		}
	}
}

void GameClient::OnLeaderboardScoresDownloaded(LeaderboardScoresDownloaded_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure) {
		if (SteamUserStats()) {
			std::cout << "Leaderboards" << std::endl;
			for (auto i = 0; i < pCallback->m_cEntryCount; ++i) {
				LeaderboardEntry_t Entry;
				std::vector<int32> details(k_cLeaderboardDetailsMax);
				if (SteamUserStats()->GetDownloadedLeaderboardEntry(pCallback->m_hSteamLeaderboardEntries, i, &Entry, details.data(), static_cast<int>(details.size()))) {
					std::cout << "\t[" << std::setfill('0') << std::setw(2) << Entry.m_nGlobalRank << "] ";
					std::cout << std::setfill('0') << std::setw(3) << Entry.m_nScore << " ... ";
					std::cout << SteamFriends()->GetFriendPersonaName(Entry.m_steamIDUser);
					//!< �A�b�v���[�h���� details ���w�肵�Ă���ꍇ
					if (Entry.m_cDetails) {
						std::cout << std::endl;
						std::cout << "Details : ";
						details.resize(Entry.m_cDetails);
						details.shrink_to_fit();
						for (auto j : details) {
							std::cout << j << ", ";
						}
					}
					std::cout << std::endl;

					//!< UGC���A�^�b�`����Ă���ꍇ�͂����UGC�̎擾������
					if (k_UGCHandleInvalid != Entry.m_hUGC) {
						if (SteamRemoteStorage()) {
							const auto Handle = SteamRemoteStorage()->UGCDownload(Entry.m_hUGC, 0);
							if (k_uAPICallInvalid != Handle) {
								mRemoteStorageDownloadUGCResult.Set(Handle, this, &GameClient::OnRemoteStorageDownloadUGCResult);
							}
						}
					}
					else {
						//!< ���[�_�[�{�[�h�̃_�E�����[�h������
						std::cout << "Leaderboard Download Completed" << std::endl;
					}
				}
			}
		}
	}
}

void GameClient::OnRemoteStorageDownloadUGCResult(RemoteStorageDownloadUGCResult_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		std::cout << "UGC downloaded" << std::endl;
		if (SteamFriends()) {
			pCallback->m_nAppID;
			std::cout << "\tOwner = " << SteamFriends()->GetFriendPersonaName(CSteamID(pCallback->m_ulSteamIDOwner)) << std::endl;
			std::cout << "\tFileName = " << pCallback->m_pchFileName << " (" << pCallback->m_nSizeInBytes << " bytes)" << std::endl;

			auto* Buffer = new uint8[pCallback->m_nSizeInBytes];
			SteamRemoteStorage()->UGCRead(pCallback->m_hFile, Buffer, pCallback->m_nSizeInBytes, 0, EUGCReadAction::k_EUGCRead_Close);

			//!< uint32 0xdeadbeef ���������܂�Ă���͂�
			uint32 data = *reinterpret_cast<uint32*>(Buffer);
			std::cout << "\t" << "0x" << std::hex << data << std::dec << std::endl;

			delete[] Buffer;

			//!< ���[�_�[�{�[�h�̃_�E�����[�h������
			std::cout << "Leaderboard Download Completed" << std::endl;
		}
	}
}

void GameClient::OnLeaderboardScoreUploaded(LeaderboardScoreUploaded_t *pCallback, bool bIOFailure)
{
	//!< �x�X�g�X�R�A���X�V�������̂� m_bScoreChanged �� true �ɂȂ�̂ŁA�����ł͌��Ȃ�
	if (!bIOFailure && pCallback->m_bSuccess /*&& pCallback->m_bScoreChanged*/) {
		std::cout << "Leaderboard Uploaded" << std::endl;

		std::cout << "\tScore = " << pCallback->m_nScore << ", ";
		std::cout << "Rank = " << pCallback->m_nGlobalRankPrevious << " -> " << pCallback->m_nGlobalRankNew << std::endl;

		if (SteamUserStats() && SteamRemoteStorage()) {
			const auto LeaderboardName = SteamUserStats()->GetLeaderboardName(pCallback->m_hSteamLeaderboard);

			//!< �Ƃ肠���� uint32 0xdeadbeef ����������
			uint32 data = 0xdeadbeef;
			//!< ���[�_�[�{�[�h���̃����[�g�t�@�C���֏�������
			if (SteamRemoteStorage()->FileWrite(LeaderboardName, &data, sizeof(data))) {
				//!< �V�F�A����
				const auto Handle = SteamRemoteStorage()->FileShare(LeaderboardName);
				if (k_uAPICallInvalid != Handle) {
					//!< m_hSteamLeaderboard �͈����ɓn����Ȃ��̂ŁA�o���Ă���
					mSteamLeaderboard = pCallback->m_hSteamLeaderboard;
					mRemoteStorageFileShareResult.Set(Handle, this, &GameClient::OnRemoteStorageFileShareResult);
				}
			}
		}
	}
}

void GameClient::OnRemoteStorageFileShareResult(RemoteStorageFileShareResult_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		std::cout << "File Shared" << std::endl;
		std::cout << "\t" << pCallback->m_rgchFilename << std::endl;

		//!< �V�F�A�������������[�g�t�@�C�������[�_�[�{�[�h�փA�^�b�`����
		if (k_UGCHandleInvalid != pCallback->m_hFile) {
			const auto Handle = SteamUserStats()->AttachLeaderboardUGC(mSteamLeaderboard, pCallback->m_hFile);
			if (k_uAPICallInvalid != Handle) {
				mLeaderboardUGCSet.Set(Handle, this, &GameClient::OnLeaderboardUGCSet);
			}
		}
	}
}

void GameClient::OnLeaderboardUGCSet(LeaderboardUGCSet_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		std::cout << "UGC Attached" << std::endl;

		//!< ���[�_�[�{�[�h�̃A�^�b�`������
		std::cout << "Leaderboard Upload Completed" << std::endl;

		//!< ���[�_�[�{�[�h�ւ̃A�^�b�`�����������̂ŁA(�e���|�������p����)�����[�g�t�@�C���͏����Ă��܂��ėǂ�
		const auto LeaderboardName = SteamUserStats()->GetLeaderboardName(pCallback->m_hSteamLeaderboard);
		SteamRemoteStorage()->FileDelete(LeaderboardName);
		std::cout << "Delete remote file " << LeaderboardName << std::endl;

		//!< ���[�_�[�{�[�h�̃_�E�����[�h���J�n
		DownloadLeaderboard("MyLeaderboard");
	}
}

void GameClient::UploadLeaderboard(const char* LeaderboardName)
{
	const auto Handle = SteamUserStats()->FindOrCreateLeaderboard(LeaderboardName, ELeaderboardSortMethod::k_ELeaderboardSortMethodDescending, ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric);
	//const auto Handle = SteamUserStats()->FindOrCreateLeaderboard(LeaderboardName, ELeaderboardSortMethod::k_ELeaderboardSortMethodAscending, ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric);
	if (k_uAPICallInvalid != Handle) {
		mLeaderboardFindResult.Set(Handle, this, &GameClient::OnLeaderboardFindResultAndUpload);
	}
}
void GameClient::DownloadLeaderboard(const char* LeaderboardName)
{
	const auto Handle = SteamUserStats()->FindOrCreateLeaderboard(LeaderboardName, ELeaderboardSortMethod::k_ELeaderboardSortMethodDescending, ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric);
	//const auto Handle = SteamUserStats()->FindOrCreateLeaderboard(LeaderboardName, ELeaderboardSortMethod::k_ELeaderboardSortMethodAscending, ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric);
	if (k_uAPICallInvalid != Handle) {
		mLeaderboardFindResult.Set(Handle, this, &GameClient::OnLeaderboardFindResultAndDownload);
	}
}

void GameClient::OnDlcInstalled(DlcInstalled_t* pCallback)
{
	std::cout << "DLC " << pCallback->m_nAppID << " is installed" << std::endl;
}
void GameClient::QueryDLC()
{
	std::cout << "DLC" << std::endl;
	if (SteamApps()) {
		for (auto i = 0; i < SteamApps()->GetDLCCount(); ++i) {
			AppId_t AppID;
			bool bAvailable;
			char Name[128];
			if (SteamApps()->BGetDLCDataByIndex(i, &AppID, &bAvailable, Name, sizeof(Name))) {
				std::cout << "\t" << Name << "(" << AppID << "), Available = " << (bAvailable ? "true" : "false") << std::endl;
				if (SteamApps()->BIsSubscribedApp(AppID)) {
					if (SteamApps()->BIsDlcInstalled(AppID)) {
						SteamApps()->UninstallDLC(AppID);
					}
					else {
						SteamApps()->InstallDLC(AppID);
						uint64 Downloaded, Total;
						if (SteamApps()->GetDlcDownloadProgress(AppID, &Downloaded, &Total)) {
						}
					}
				}
			}
		}
	}
}

void GameClient::OnCreateItemResult(CreateItemResult_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		std::cout << "UGC created" << std::endl;
		
		//!< �_��ɓ��ӂ�����Ƃ��̏���
		if (pCallback->m_bUserNeedsToAcceptWorkshopLegalAgreement) {
			const auto Friend = SteamFriends();
			if (nullptr != Friend) {
				Friend->ActivateGameOverlayToWebPage((std::string("steam://url/CommunityFilePage/") + std::to_string(pCallback->m_nPublishedFileId)).c_str());
			}
		}

		const auto UGC = SteamUGC();
		const auto Util = SteamUtils();
		if (nullptr != UGC && nullptr != Util) {
			const auto AppID = Util->GetAppID();
			
			//!< (�쐬����)�A�C�e���̃R���e���c���w�肷��
			auto UpdateHandle = SteamUGC()->StartItemUpdate(AppID, pCallback->m_nPublishedFileId);
			if (k_uAPICallInvalid != UpdateHandle) {
				if (!UGC->SetItemTitle(UpdateHandle, "UGC Title")) {
					std::cout << "SetItemTitle() failed" << std::endl;
				}
				if (!UGC->SetItemDescription(UpdateHandle, "UGC Description")) {
					std::cout << "SetItemDescription() failed" << std::endl;
				}
				//if (!UGC->SetItemPreview(UpdateHandle, "D:/GitHub/Steamworks/Steam/Preview.JPG")) { //!< 1MB�����Ə����Ă��邪�����Ə������Ȃ��Ă̓_���������H
				//	std::cout << "SetItemPreview() failed" << std::endl;
				//}
				
				//!< �u��΃p�X�v���w�肷�邱��
				char Fullpath[BUFSIZ];
				if (GetFullPathNameA("./UGC", sizeof(Fullpath), Fullpath, nullptr)) {
					if (!UGC->SetItemContent(UpdateHandle, Fullpath)) {
						std::cout << "SetItemContent() failed" << std::endl;
					}
				}
				if (!UGC->SetItemMetadata(UpdateHandle, "This is Metadata")) {
					std::cout << "SetItemMetadata() failed" << std::endl;
				}

				//!< �폜�������ꍇ : �폜������@�͖����̂ŁA�p�u���b�N���猩���Ȃ��悤�ɂ��邱�Ƃő�p
				//UGC->SetItemVisibility(UpdateHandle, ERemoteStoragePublishedFileVisibility::k_ERemoteStoragePublishedFileVisibilityPrivate);

				//!< �T�u�~�b�g����
				auto SubmitHandle = UGC->SubmitItemUpdate(UpdateHandle, nullptr);
				if (k_uAPICallInvalid != SubmitHandle) {
					mSubmitItemUpdateResult.Set(SubmitHandle, this, &GameClient::OnSubmitItemUpdateResult);

					//!< �p�u���b�V���t�@�C��ID���o���Ă���
					mPublishedFileId = pCallback->m_nPublishedFileId;
				}
			}
		}
	}
}
void GameClient::OnSubmitItemUpdateResult(SubmitItemUpdateResult_t* pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		if (pCallback->m_bUserNeedsToAcceptWorkshopLegalAgreement) {}

		std::cout << "UGC sumbitted" << std::endl;

		//!< �����ł�(�T�u�~�b�g����)�A�C�e��������T�u�X�N���C�u����
		const auto UGC = SteamUGC();
		if (nullptr != UGC) {
			auto Handle = UGC->SubscribeItem(mPublishedFileId);
			if (k_uAPICallInvalid != Handle) {
				mRemoteStorageSubscribePublishedFileResult.Set(Handle, this, &GameClient::OnRemoteStorageSubscribePublishedFileResult);
			}
		}
	}
}
void GameClient::OnRemoteStorageSubscribePublishedFileResult(RemoteStorageSubscribePublishedFileResult_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		std::cout << "UGC subscribed : " << pCallback->m_nPublishedFileId << std::endl;

#if 0
		//!< �A���T�u�X�N���C�u����ꍇ�͂���Ȋ����ɂ���
		const auto UGC = SteamUGC();
		if (nullptr != UGC) {
			auto Handle = UGC->SubscribeItem(pCallback->m_nPublishedFileId);
			if (k_uAPICallInvalid != Handle) {
				mRemoteStorageUnsubscribePublishedFileResult.Set(Handle, this, &GameClient::OnRemoteStorageUnsubscribePublishedFileResult);
			}
		}
#endif
	}
}
void GameClient::OnRemoteStorageUnsubscribePublishedFileResult(RemoteStorageUnsubscribePublishedFileResult_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		std::cout << "UGC unsubscribed : " << pCallback->m_nPublishedFileId << std::endl;
	}
}

void GameClient::OnSteamUGCQueryCompleted(SteamUGCQueryCompleted_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		std::cout << "UGC query completed : " << pCallback->m_unNumResultsReturned << " / " << pCallback->m_unTotalMatchingResults << std::endl;

		if (pCallback->m_unNumResultsReturned) {
			const auto UGC = SteamUGC();
			if (nullptr != UGC) {
				for (uint32 i = 0; i < pCallback->m_unNumResultsReturned; ++i) {
					SteamUGCDetails_t Details;
					if (UGC->GetQueryUGCResult(pCallback->m_handle, i, &Details)) {
						std::cout << "\tFileID : " << Details.m_nPublishedFileId << std::endl;
						
						if (EWorkshopFileType::k_EWorkshopFileTypeCommunity == Details.m_eFileType) {
							std::cout << "\tFileType : Community" << std::endl;
						}
						else if (EWorkshopFileType::k_EWorkshopFileTypeGameManagedItem == Details.m_eFileType) {
							std::cout << "\tFileType : GameManaged" << std::endl;
						}
						else {
							std::cout << "\tFileType : " << Details.m_eFileType << std::endl;
						}

						std::cout << "\tTitle : " << Details.m_rgchTitle << std::endl;
						std::cout << "\tDescrition : " << Details.m_rgchDescription << std::endl;
						std::cout << "\tSize : " << Details.m_nFileSize << std::endl;
						std::cout << std::endl;
					}
				}
			}
		}
	}
}

void GameClient::OnRemoteStoragePublishedFileSubscribed(RemoteStoragePublishedFileSubscribed_t *pCallback)
{
	//!< �O������T�u�X�N���C�u���ꂽ (�A�v���Ɋ֌W�Ȃ��Ăяo�����̂� AppID ���`�F�b�N���邱��)
	const auto Util = SteamUtils();
	if (nullptr != Util && Util->GetAppID() == pCallback->m_nAppID) {
		std::cout << "UGC subscribed : " << pCallback->m_nPublishedFileId << std::endl;
	}
}
void GameClient::OnRemoteStoragePublishedFileUnsubscribed(RemoteStoragePublishedFileUnsubscribed_t *pCallback)
{
	//!< �O������A���T�u�X�N���C�u���ꂽ (�A�v���Ɋ֌W�Ȃ��Ăяo�����̂� AppID ���`�F�b�N���邱��)
	const auto Util = SteamUtils();
	if (nullptr != Util && Util->GetAppID() == pCallback->m_nAppID) {
		std::cout << "UGC unsubscribed : " << pCallback->m_nPublishedFileId << std::endl;
	}
}
//void GameClient::OnRemoteStorageEnumerateUserPublishedFilesResult(RemoteStorageEnumerateUserPublishedFilesResult_t* pCallback){}
//void GameClient::OnRemoteStorageEnumerateUserSubscribedFilesResult(RemoteStorageEnumerateUserSubscribedFilesResult_t* pCallback){}
//void GameClient::OnRemoteStorageEnumerateWorkshopFilesResult(RemoteStorageEnumerateWorkshopFilesResult_t* pCallback){}
//void GameClient::OnRemoteStorageEnumerateUserSharedWorkshopFilesResult(RemoteStorageEnumerateUserSharedWorkshopFilesResult_t* pCallback){}
//void GameClient::OnRemoteStorageEnumeratePublishedFilesByUserActionResult(RemoteStorageEnumeratePublishedFilesByUserActionResult_t* pCallback){}

void GameClient::OnItemInstalled(ItemInstalled_t* pCallback)
{
	const auto Util = SteamUtils();
	if (nullptr != Util && Util->GetAppID() == pCallback->m_unAppID) {
		const auto UGC = SteamUGC();
		if (nullptr != UGC) {
			uint64 Size;
			char Folder[BUFSIZ];
			uint32 TimeStamp;
			if (UGC->GetItemInstallInfo(pCallback->m_nPublishedFileId, &Size, Folder, sizeof(Folder), &TimeStamp)) {
				//!< ���̃t�H���_�� SteamUGC()->SetItemContent() �Ŏw�肵���t�H���_�ȉ��̂��̂��C���X�g�[������Ă���
				std::cout << "UGC " << pCallback->m_nPublishedFileId << " is installed in : " << Folder << std::endl;
			}
		}
	}
}
void GameClient::CreateUGC()
{
	const auto UGC = SteamUGC();
	const auto Util = SteamUtils();
	if (nullptr != UGC && nullptr != Util) {
		//!< �A�C�e���쐬
		const auto AppID = Util->GetAppID();

		const auto FileType = EWorkshopFileType::k_EWorkshopFileTypeGameManagedItem;
		//const auto FileType = EWorkshopFileType::k_EWorkshopFileTypeCommunity;

		auto Handle = UGC->CreateItem(AppID, FileType);
		if (k_uAPICallInvalid != Handle) {
			mCreateItemResult.Set(Handle, this, &GameClient::OnCreateItemResult);
		}
	}
}

void GameClient::QueryUGC()
{
	const auto UGC = SteamUGC();
	const auto User = SteamUser();
	const auto Util = SteamUtils();
	if (nullptr != UGC && nullptr != User && nullptr != Util) {
		const auto AccountID = User->GetSteamID().GetAccountID();
		const auto AppID = Util->GetAppID();
		
		const auto ListType = EUserUGCList::k_EUserUGCList_Published;
		//const auto ListType = EUserUGCList::k_EUserUGCList_Subscribed;
		
		const auto MatchingType = EUGCMatchingUGCType::k_EUGCMatchingUGCType_All;
		//const auto MathcingType = EUGCMatchingUGCType::k_EUGCMatchingUGCType_GameManagedItems;

		const auto QueryHandle = UGC->CreateQueryUserUGCRequest(AccountID,
			ListType,
			MatchingType,
			EUserUGCListSortOrder::k_EUserUGCListSortOrder_CreationOrderAsc,
			AppID, AppID, 1);

		if (k_UGCQueryHandleInvalid != QueryHandle) {
			auto Handle = UGC->SendQueryUGCRequest(QueryHandle);
			if (k_uAPICallInvalid != Handle) {
				mSteamUGCQueryCompleted.Set(Handle, this, &GameClient::OnSteamUGCQueryCompleted);
			}
		}
	}
}

void GameClient::OnRemoteStorageFileReadAsyncComplete(RemoteStorageFileReadAsyncComplete_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		auto RemoteStorage = SteamRemoteStorage();
		if (nullptr != RemoteStorage) {
			mRemoteStorageBuffer.resize(pCallback->m_nOffset + pCallback->m_cubRead);
			RemoteStorage->FileReadAsyncComplete(pCallback->m_hFileReadAsync, mRemoteStorageBuffer.data() + pCallback->m_nOffset, pCallback->m_cubRead);
			std::cout << "RemoteStorage Read : Size = " << mRemoteStorageBuffer.size() << ", Value = " << static_cast<uint32>(mRemoteStorageBuffer[0]) << std::endl;
		}
	}
}
void GameClient::OnRemoteStorageFileWriteAsyncComplete(RemoteStorageFileWriteAsyncComplete_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure && k_EResultOK == pCallback->m_eResult) {
		ReadRemoteStorage();
	}
}
void GameClient::QueryRemoteStoage()
{
	uint64 totalBytes, availableBytes;
	if (SteamRemoteStorage()->GetQuota(&totalBytes, &availableBytes)) {
		std::cout << "TotalBytes = " << totalBytes << ", AvailableBytes = " << availableBytes << std::endl;
	}

	std::cout << "IsCloudEnabledForAccount = " << (SteamRemoteStorage()->IsCloudEnabledForAccount() ? "true" : "false") << std::endl;
	std::cout << "IsCloudEnabledForApp = " << (SteamRemoteStorage()->IsCloudEnabledForApp() ? "true" : "false") << std::endl;

	for (auto i = 0; i < SteamRemoteStorage()->GetFileCount(); ++i) {
		int32 Size;
		const auto Name = SteamRemoteStorage()->GetFileNameAndSize(i, &Size);
		std::cout << "[" << i << "] : " << Name << "(Size = " << Size << ")" << std::endl;
	}
	std::cout << std::endl;

#if 0
	//!< �����[�g�t�@�C�������ׂď����ꍇ(���[�J���A�����[�g��)
	//!< FileDelete() ����� GetFileCount() �͂��̏�Ō����Ă����̂ŁA��U�u�t�@�C�����̃��X�g�v������Ă����������
	//!< �����[�g�̂ݏ���(FileForget())�ꍇ�� GetFileCount() �͌���Ȃ��̂ŁA���̈��Ԃ͂���Ȃ�
	const auto Count = SteamRemoteStorage()->GetFileCount();
	if (Count) {
		std::vector<std::string> Names(Count);
		for (auto i = 0; i < SteamRemoteStorage()->GetFileCount(); ++i) {
			int32 Size;
			Names.push_back(SteamRemoteStorage()->GetFileNameAndSize(i, &Size));
		}
		for (auto i : Names) {
			SteamRemoteStorage()->FileDelete(i.c_str());
		}
		assert(0 == SteamRemoteStorage()->GetFileCount() && "");
	}
#endif

	//ReadRemmoteStorage();
	//WriteRemoteStorage();
}
void GameClient::WriteRemoteStorage()
{
	auto RemoteStorage = SteamRemoteStorage();
	if (nullptr != RemoteStorage) {

		std::random_device rd;
		const int32 Size = (rd() % 32) + 1;
		const int32 Value = rd() % 0xff;
		mRemoteStorageBuffer.assign(Size, Value);
		std::cout << "RemoteStorage Write : Size = " << Size << ", Value = " << Value << std::endl;

		if (k_unMaxCloudFileChunkSize >= static_cast<uint32>(mRemoteStorageBuffer.size())) {
			const auto Handle = RemoteStorage->FileWriteAsync(mRemoteStorageFile.c_str(), mRemoteStorageBuffer.data(), static_cast<uint32>(mRemoteStorageBuffer.size()));
			if (k_uAPICallInvalid != Handle) {
				mRemoteStorageFileWriteAsyncComplete.Set(Handle, this, &GameClient::OnRemoteStorageFileWriteAsyncComplete);
			}
		}
		else {
			auto Handle = RemoteStorage->FileWriteStreamOpen(mRemoteStorageFile.c_str());
			if (k_UGCFileStreamHandleInvalid != Handle) {
				auto ChunkData = reinterpret_cast<uint8*>(mRemoteStorageBuffer.data());
				auto ChunkSize = static_cast<uint32>(mRemoteStorageBuffer.size());
				while (ChunkSize > 0) {
					if (RemoteStorage->FileWriteStreamWriteChunk(Handle, ChunkData, (std::min)(ChunkSize, k_unMaxCloudFileChunkSize))) {
					}
					ChunkSize -= k_unMaxCloudFileChunkSize;
					ChunkData += k_unMaxCloudFileChunkSize;
				}
			}
		}
	}
}
void GameClient::ReadRemoteStorage()
{
	auto RemoteStorage = SteamRemoteStorage();
	if (nullptr != RemoteStorage) {
		if (RemoteStorage->FileExists(mRemoteStorageFile.c_str())) {
			const uint32 Offset = 0;
			const auto Size = RemoteStorage->GetFileSize(mRemoteStorageFile.c_str());
			auto Handle = RemoteStorage->FileReadAsync(mRemoteStorageFile.c_str(), Offset, Size);
			if (k_uAPICallInvalid != Handle) {
				mRemoteStorageFileReadAsyncComplete.Set(Handle, this, &GameClient::OnRemoteStorageFileReadAsyncComplete);
			}
		}
	}
}

void GameClient::GetFriendInformation()
{
	if (SteamFriends()) {
		for (auto i = 0; i < SteamFriends()->GetFriendCount(k_EFriendFlagAll); ++i) {
			const auto SteamID = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagAll);

			//!< �y���\�i���A�A�o�^�[���擾����
			const auto bNameOnly = false; //!< �A�o�^�[�����[�h����
			if (SteamFriends()->RequestUserInformation(SteamID, bNameOnly)) {
				//!< PersonaStateChanged_t ���R�[���o�b�N�����

				//!< �Ƃ肠�����ŏ��̈�l�������[�U���(�ƃA�o�^�[)�����N�G�X�g���� break
				std::cout << "\t" << "Requesting ... " << SteamFriends()->GetFriendPersonaName(SteamID) << "'s user information" << std::endl;
				break;
			}
			else {
				//!< ���[�U�������Ɏ擾�ł��Ă���

				PrintPersonaName(SteamID);
				PrintNickname(SteamID);
				PrintSteamLevel(SteamID);
				PrintPersonaState(SteamID);
				PrintGamePlayed(SteamID);
				PrintAvatarInfo(SteamID);
			}
		}
	}

	PrintFriendRelationship();
}

void GameClient::PrintFriendRelationship()
{
	if (SteamFriends()) {
		std::cout << "All Friends" << std::endl;
		auto bRequestUserInformation = false;
		for (auto i = 0; i < SteamFriends()->GetFriendCount(k_EFriendFlagAll); ++i) {
			const auto SteamID = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagAll);
			if (SteamID.IsValid()) {
				std::cout << "\t" << SteamFriends()->GetFriendPersonaName(SteamID) << std::endl;

				switch (SteamFriends()->GetFriendRelationship(SteamID)) {
				case k_EFriendRelationshipNone:
				case k_EFriendRelationshipFriend:
				case k_EFriendRelationshipIgnored:
				case k_EFriendRelationshipIgnoredFriend:
					break;
				}
			}
		}

		std::cout << "Ignores" << std::endl;
		for (auto i = 0; i < SteamFriends()->GetFriendCount(k_EFriendFlagIgnored); ++i) {
			const auto SteamID = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagIgnored);
			if (SteamID.IsValid()) {
				std::cout << "\t" << SteamFriends()->GetFriendPersonaName(SteamID) << std::endl;
			}
		}

		std::cout << "Ignore Friends" << std::endl;
		for (auto i = 0; i < SteamFriends()->GetFriendCount(k_EFriendFlagIgnoredFriend); ++i) {
			const auto SteamID = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagIgnoredFriend);
			if (SteamID.IsValid()) {
				std::cout << "\t" << SteamFriends()->GetFriendPersonaName(SteamID) << std::endl;
			}
		}

		std::cout << "Coplay" << std::endl;
		for (auto i = 0; i < SteamFriends()->GetCoplayFriendCount(); ++i) {
			const auto SteamID = SteamFriends()->GetCoplayFriend(i);
			if (SteamID.IsValid() && SteamID != SteamUser()->GetSteamID()) {
				const auto UnixTime = static_cast<time_t>(SteamFriends()->GetFriendCoplayTime(SteamID));
				const auto LocalTime = localtime(&UnixTime);
				const auto GreenwichTime = gmtime(&UnixTime);
				std::cout << "\t" << asctime(LocalTime);
				std::cout << "\t" << "GameID = " << SteamFriends()->GetFriendCoplayGame(SteamID) << std::endl;
				std::cout << "\t" << SteamFriends()->GetFriendPersonaName(SteamID) << std::endl;
			}
		}
	}
}

void GameClient::PrintPersonaName(const CSteamID SteamID)
{
	std::cout << "\t" << "\t" << "PersonaName" << std::endl;
	std::cout << "\t" << "\t" << "\t" << SteamFriends()->GetFriendPersonaName(SteamID) << std::endl;

	
}

void GameClient::PrintNickname(const CSteamID SteamID)
{
	const auto Nickname = SteamFriends()->GetPlayerNickname(SteamID);
	if (nullptr != Nickname) {
		std::cout << "\t" << "\t" << "NickName" << std::endl;
		std::cout << "\t" << "\t" << "\t" << Nickname << std::endl;
	}
}

void GameClient::PrintSteamLevel(const CSteamID SteamID)
{
	std::cout << "\t" << "\t" << "SteamLevel" << std::endl;
	std::cout << "\t" << "\t" << "\t" << SteamFriends()->GetFriendSteamLevel(SteamID) << std::endl;
}

void GameClient::PrintPersonaState(const CSteamID SteamID)
{
	std::cout << "\t" << "\t" << "PersonaState" << std::endl;
	const auto PersonalState = SteamFriends()->GetFriendPersonaState(SteamID);
	switch (PersonalState) {
	case k_EPersonaStateOffline: std::cout << "\t" << "\t" << "\t" << "Offline" << std::endl; break;
	case k_EPersonaStateOnline:  std::cout << "\t" << "\t" << "\t" << "Online" << std::endl; break;
	}
}
void GameClient::PrintGamePlayed(const CSteamID SteamID)
{
	FriendGameInfo_t FriendGameInfo;
	if (SteamFriends()->GetFriendGamePlayed(SteamID, &FriendGameInfo)) {
		std::cout << "\t" << "\t" << "GamePlayed" << std::endl;
		std::cout << "\t" << "\t" << "\t" << FriendGameInfo.m_gameID.ToUint64() << std::endl;
	}
}
void GameClient::PrintAvatarInfo(const CSteamID SteamID)
{
	std::cout << "\t" << "\t" << "Avatar" << std::endl;
	if (SteamFriends()) {
		//!< ���A�o�^�[�̎擾
		auto ImageHandle = SteamFriends()->GetSmallFriendAvatar(SteamID);
		if (ImageHandle) {
			OnImageLoad(ImageHandle);
		}

		//!< ���A�o�^�[�̎擾
		ImageHandle = SteamFriends()->GetMediumFriendAvatar(SteamID);
		if (ImageHandle) {
			OnImageLoad(ImageHandle);
		}

		//!< ��A�o�^�[�̎擾(���N�G�X�g)
		ImageHandle = SteamFriends()->GetLargeFriendAvatar(SteamID);
		if (-1 == ImageHandle) {
			//!< -1 �̏ꍇ�̓��N�G�X�g�����AAvatarImageLoaded_t �ŃR�[���o�b�N�����
			std::cout << "\t" << "\t" << "\t" << "Requesting large avatar ..." << std::endl;
		}
		else if (ImageHandle) {
			OnImageLoad(ImageHandle);
		}
	}
}

void GameClient::OnImageLoad(const int ImageHandle)
{
	uint32 Width, Height;
	if (SteamUtils()->GetImageSize(ImageHandle, &Width, &Height)) {
		std::vector<uint8> Image(4 * Height * Width);
		if (SteamUtils()->GetImageRGBA(ImageHandle, Image.data(), static_cast<int>(Image.size()))) {
			//!< �摜�����[�h���ꂽ
			std::cout << "\t" << "\t" << "\t" << "Image(RGBA) loaded " << Width << " x " << Height << std::endl;
		}
	}
}

void GameClient::OnPersonaStateChange(PersonaStateChange_t* pCallback)
{
	std::cout << "OnPersonaStateChange()" << std::endl;

	const auto SteamID = CSteamID(pCallback->m_ulSteamID);

	if (k_EPersonaChangeName & pCallback->m_nChangeFlags) {
		PrintPersonaName(SteamID);
	}
	if (k_EPersonaChangeStatus & pCallback->m_nChangeFlags) {
		PrintPersonaState(SteamID);
	}
	if (k_EPersonaChangeComeOnline & pCallback->m_nChangeFlags) {
	}
	if (k_EPersonaChangeGoneOffline & pCallback->m_nChangeFlags) {
	}
	if (k_EPersonaChangeGamePlayed & pCallback->m_nChangeFlags) {
		PrintGamePlayed(SteamID);
	}
	if (k_EPersonaChangeGameServer & pCallback->m_nChangeFlags) {}
	if (k_EPersonaChangeAvatar & pCallback->m_nChangeFlags) {
		PrintAvatarInfo(SteamID);
	}
	if (k_EPersonaChangeJoinedSource & pCallback->m_nChangeFlags) {}
	if (k_EPersonaChangeLeftSource & pCallback->m_nChangeFlags) {}
	if (k_EPersonaChangeRelationshipChanged & pCallback->m_nChangeFlags) {
		PrintFriendRelationship();
	}
	if (k_EPersonaChangeNameFirstSet & pCallback->m_nChangeFlags) {}
	if (k_EPersonaChangeFacebookInfo & pCallback->m_nChangeFlags) {}
	if (k_EPersonaChangeNickname & pCallback->m_nChangeFlags) {
		PrintNickname(SteamID);
	}
	if (k_EPersonaChangeSteamLevel & pCallback->m_nChangeFlags) {
		PrintSteamLevel(SteamID);
	}
}
void GameClient::OnAvatarImageLoaded(AvatarImageLoaded_t* pCallback)
{
	std::cout << "OnAvatarImageLoaded()" << std::endl;

	if (pCallback->m_iImage) {
		OnImageLoad(pCallback->m_iImage);
	}
}
void GameClient::OnGameOverlayActivated(GameOverlayActivated_t* pCallback)
{
	std::cout << "OnGameOverlayActivated()" << std::endl;

	//!< DirectX ���ŕ`�悳��Ă��Ȃ��ƃI�[�o�[���C�͕\������Ȃ�
	if (pCallback->m_bActive) {}
}
#pragma endregion

#ifdef USE_DX
void DX::OnCreate(HWND hWnd)
{
	CreateDevice(hWnd);
	CreateCommandQueue();
	CreateFence();
	CreateSwapchain(hWnd);
	CreateRootSignature();
	//CreatePipelineState();
}

void DX::OnSize(HWND hWnd, HINSTANCE hInstance)
{
	WaitForFence();

	RECT Rect;
	GetClientRect(hWnd, &Rect);
	const auto Width = static_cast<UINT>(Rect.right - Rect.left);
	const auto Height = static_cast<UINT>(Rect.bottom - Rect.top);
	CreateViewport(static_cast<FLOAT>(Width), static_cast<FLOAT>(Height));

	

	for (auto i = 0; i < GraphicsCommandLists.size(); ++i) {
		PopulateCommandList(i);
	}
}

void DX::OnPaint(HWND hWnd, HINSTANCE hInstance)
{
	Draw();
}

void DX::OnDestroy(HWND hWnd, HINSTANCE hInstance)
{
	WaitForFence();
}

void DX::CreateDevice(HWND hWnd)
{
	using namespace Microsoft::WRL;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> Debug;
	VERIFY_SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(Debug.GetAddressOf())));
	Debug->EnableDebugLayer();
#endif

	ComPtr<IDXGIFactory4> Factory;
	VERIFY_SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(Factory.GetAddressOf())));

	ComPtr<IDXGIAdapter> Adapter;
	Factory->EnumAdapters(0, Adapter.ReleaseAndGetAddressOf());

	const std::vector<D3D_FEATURE_LEVEL> FeatureLevels = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	auto FeatureLevel = D3D_FEATURE_LEVEL_9_1;
	for (const auto i : FeatureLevels) {
		if (SUCCEEDED(D3D12CreateDevice(Adapter.Get(), i, _uuidof(ID3D12Device), nullptr))) {
			FeatureLevel = i;
			break;
		}
	}
	D3D12CreateDevice(Adapter.Get(), FeatureLevel, IID_PPV_ARGS(Device.GetAddressOf()));
}

void DX::CreateCommandQueue()
{
	const D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		0,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};
	VERIFY_SUCCEEDED(Device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(CommandQueue.GetAddressOf())));
}

void DX::CreateFence()
{
	VERIFY_SUCCEEDED(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(Fence.GetAddressOf())));
}

void DX::CreateSwapchain(HWND hWnd)
{
	RECT Rect;
	GetClientRect(hWnd, &Rect);
	const auto Width = static_cast<UINT>(Rect.right - Rect.left);
	const auto Height = static_cast<UINT>(Rect.bottom - Rect.top);

	using namespace Microsoft::WRL;

	const UINT BufferCount = 3;

	ComPtr<IDXGIFactory4> Factory;
	VERIFY_SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(Factory.GetAddressOf())));

	SwapChain.Reset();
	ComPtr<IDXGISwapChain> NewSwapChain;
	const DXGI_RATIONAL Rational = { 60, 1 };
	const DXGI_MODE_DESC ModeDesc = {
		Width, Height,
		Rational,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		DXGI_MODE_SCALING_UNSPECIFIED
	};
	const DXGI_SAMPLE_DESC SampleDesc = { 1, 0 };
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {
		ModeDesc,
		SampleDesc,
		DXGI_USAGE_RENDER_TARGET_OUTPUT,
		BufferCount,
		hWnd,
		TRUE,
		DXGI_SWAP_EFFECT_FLIP_DISCARD,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};
	VERIFY_SUCCEEDED(Factory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, NewSwapChain.GetAddressOf()));
	VERIFY_SUCCEEDED(NewSwapChain.As(&SwapChain));

	const D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc = {
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		SwapChainDesc.BufferCount,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};
	VERIFY_SUCCEEDED(Device->CreateDescriptorHeap(&DescriptorHeapDesc, IID_PPV_ARGS(SwapChainDescriptorHeap.GetAddressOf())));

	CreateSwapChainResource();

	CreateCommandList();
}

void DX::CreateCommandList()
{
	VERIFY_SUCCEEDED(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CommandAllocator.GetAddressOf())));

	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc;
	SwapChain->GetDesc1(&SwapChainDesc);
	for (UINT i = 0; i < SwapChainDesc.BufferCount; ++i) {
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;
		VERIFY_SUCCEEDED(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), nullptr, IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));

		GraphicsCommandLists.push_back(GraphicsCommandList);
		VERIFY_SUCCEEDED(GraphicsCommandLists.back()->Close());
	}
}

void DX::CreateSwapChainResource()
{
	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc;
	SwapChain->GetDesc1(&SwapChainDesc);
	SwapChainResources.resize(SwapChainDesc.BufferCount);
	for (auto i = 0; i < SwapChainResources.size(); ++i) {
		VERIFY_SUCCEEDED(SwapChain->GetBuffer(i, IID_PPV_ARGS(SwapChainResources[i].GetAddressOf())));

		const auto SCR = SwapChainResources[i].Get();

		auto CDH(SwapChainDescriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart());
		CDH.ptr += i * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		Device->CreateRenderTargetView(SCR, nullptr, CDH);
	}
}

void DX::CreateRootSignature()
{
	const std::vector<D3D12_ROOT_PARAMETER> RootParameters = {};
	const std::vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplerDescs = {};
		const D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {
		static_cast<UINT>(RootParameters.size()), RootParameters.data(),
		static_cast<UINT>(StaticSamplerDescs.size()), StaticSamplerDescs.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	};
	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob;
	VERIFY_SUCCEEDED(D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, Blob.GetAddressOf(), ErrorBlob.GetAddressOf()));
	VERIFY_SUCCEEDED(Device->CreateRootSignature(0, Blob->GetBufferPointer(), Blob->GetBufferSize(), IID_PPV_ARGS(RootSignature.GetAddressOf())));
}

void DX::CreatePipelineState()
{
	const D3D12_SHADER_BYTECODE DefaultShaderBytecode = { nullptr, 0 };
	const std::vector<D3D12_SHADER_BYTECODE> ShaderBytecodes = {
		DefaultShaderBytecode,
		DefaultShaderBytecode,
		DefaultShaderBytecode,
		DefaultShaderBytecode,
		DefaultShaderBytecode,
	};

	const D3D12_STREAM_OUTPUT_DESC StreamOutputDesc = {
		nullptr, 0,
		nullptr, 0,
		0
	};

	const D3D12_RENDER_TARGET_BLEND_DESC DefaultRenderTargetBlendDesc = {
		FALSE, FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL,
	};
	const D3D12_BLEND_DESC BlendDesc = {
		FALSE,
		FALSE,
		{ DefaultRenderTargetBlendDesc }
	};

	const D3D12_RASTERIZER_DESC RasterizerDesc = {
		D3D12_FILL_MODE_SOLID,
		D3D12_CULL_MODE_BACK, TRUE,
		D3D12_DEFAULT_DEPTH_BIAS, D3D12_DEFAULT_DEPTH_BIAS_CLAMP, D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
		TRUE,
		FALSE,
		FALSE,
		0,
		D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
	};

	const D3D12_DEPTH_STENCILOP_DESC DepthStencilOpDesc = {
		D3D12_STENCIL_OP_KEEP,
		D3D12_STENCIL_OP_KEEP,
		D3D12_STENCIL_OP_KEEP,
		D3D12_COMPARISON_FUNC_NEVER
	};

	const D3D12_DEPTH_STENCIL_DESC DepthStencilDesc = {
		FALSE,
		D3D12_DEPTH_WRITE_MASK_ZERO,
		D3D12_COMPARISON_FUNC_NEVER,
		FALSE,
		0,
		0,
		DepthStencilOpDesc,
		DepthStencilOpDesc
	};

	const std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDescs = {};
	const D3D12_INPUT_LAYOUT_DESC InputLayoutDesc = {
		InputElementDescs.data(), static_cast<UINT>(InputElementDescs.size())
	};

	const DXGI_SAMPLE_DESC SampleDesc = { 1, 0 };
	const D3D12_CACHED_PIPELINE_STATE CachedPipelineState = { nullptr, 0 };
	const D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsPipelineStateDesc = {
		RootSignature.Get(),
		ShaderBytecodes[0], ShaderBytecodes[1], ShaderBytecodes[2], ShaderBytecodes[3], ShaderBytecodes[4],
		StreamOutputDesc,
		BlendDesc,
		UINT_MAX,
		RasterizerDesc,
		DepthStencilDesc,
		InputLayoutDesc,
		D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		1,{ DXGI_FORMAT_R8G8B8A8_UNORM },
		DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
		SampleDesc,
		0,
		CachedPipelineState,
		D3D12_PIPELINE_STATE_FLAG_NONE
	};

	VERIFY_SUCCEEDED(Device->CreateGraphicsPipelineState(&GraphicsPipelineStateDesc, IID_PPV_ARGS(PipelineState.GetAddressOf())));
}

void DX::CreateViewport(const FLOAT Width, const FLOAT Height, const FLOAT MinDepth, const FLOAT MaxDepth)
{
	Viewports = {
		{
			0.0f, 0.0f,
			Width, Height,
			MinDepth, MaxDepth
		}
	};
	ScissorRects = {
		{
			0, 0,
			static_cast<LONG>(Width), static_cast<LONG>(Height)
		}
	};
}

void DX::ResourceBarrier(ID3D12GraphicsCommandList* CommandList, ID3D12Resource* Resource, const D3D12_RESOURCE_STATES Before, const D3D12_RESOURCE_STATES After)
{
	const D3D12_RESOURCE_TRANSITION_BARRIER ResourceTransitionBarrier = {
		Resource,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
		Before,
		After
	};
	const std::vector<D3D12_RESOURCE_BARRIER> ResourceBarrier = {
		{
			D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			D3D12_RESOURCE_BARRIER_FLAG_NONE,
			ResourceTransitionBarrier
		}
	};
	CommandList->ResourceBarrier(static_cast<UINT>(ResourceBarrier.size()), ResourceBarrier.data());
}

void DX::PopulateCommandList(const size_t i)
{
	const auto CL = GraphicsCommandLists[i].Get();
	const auto SCR = SwapChainResources[i].Get();
	auto CDH(SwapChainDescriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart());
	CDH.ptr += static_cast<UINT>(i) * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	const auto CA = CommandAllocator.Get();

	VERIFY_SUCCEEDED(CL->Reset(CA, PipelineState.Get()));
	{
		//!< �r���[�|�[�g�A�V�U�[
		CL->RSSetViewports(static_cast<UINT>(Viewports.size()), Viewports.data());
		CL->RSSetScissorRects(static_cast<UINT>(ScissorRects.size()), ScissorRects.data());

		ResourceBarrier(CL, SCR, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET); {
			CL->ClearRenderTargetView(CDH, DirectX::Colors::SkyBlue, 0, nullptr);
		} ResourceBarrier(CL, SCR, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	}
	VERIFY_SUCCEEDED(CL->Close());
}

void DX::Draw()
{
	WaitForFence();

	CurrentBackBufferIndex = 0xffffffff == CurrentBackBufferIndex ? SwapChain->GetCurrentBackBufferIndex() : (CurrentBackBufferIndex + 1) % static_cast<const UINT>(SwapChainResources.size());

	const std::vector<ID3D12CommandList*> CommandLists = {
		GraphicsCommandLists[CurrentBackBufferIndex].Get()
	};
	CommandQueue->ExecuteCommandLists(static_cast<UINT>(CommandLists.size()), CommandLists.data());

	VERIFY_SUCCEEDED(SwapChain->Present(1, 0));
}

void DX::WaitForFence()
{
	++FenceValue;
	VERIFY_SUCCEEDED(CommandQueue->Signal(Fence.Get(), FenceValue));
	if (Fence->GetCompletedValue() < FenceValue) {
		auto hEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
		VERIFY_SUCCEEDED(Fence->SetEventOnCompletion(FenceValue, hEvent));

		WaitForSingleObject(hEvent, INFINITE);
		CloseHandle(hEvent);
	}
}

#endif
