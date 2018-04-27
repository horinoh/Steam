// Steam.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Steam.h"

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
#pragma region AddCode
	case WM_CREATE:
		if (nullptr == SteamInst) {
			SteamInst = new Steam();
		}
		SetTimer(hWnd, NULL, 1000 / 60, nullptr);
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
#pragma endregion
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
#pragma region AddCode
		SAFE_DELETE(SteamInst);
#pragma endregion
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
		UploadLeaderboard();
		//DownloadLeaderboard();
		
		//!< UGC(User Generated Content)  �̃N�G��
		QueryUGC();

		//!< UGC(User Generated Content) �̍쐬
		//CreateUGC();

		//!< RemoteStorage �̃N�G��
		QueryRemoteStoage();
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

			//!< ���[�_�{�[�h���o���Ă���
			mSteamLeaderboard = pCallback->m_hSteamLeaderboard;

			//!< �A�b�v���[�h�J�n
			std::random_device rd;
			const int32 Score = rd() % 101;
			//!< �A�b�v���[�h���� details ���w�肷��ƁA�_�E�����[�h���Ɏ擾�ł��� (int32 * k_cLeaderboardDetailsMax(=64))
			//!< UGC�ɂ���قǂł��Ȃ��f�[�^��t�������ꍇ
			const std::vector<int32> details = { 0, 1, 2, 3 };
			const auto Handle = SteamUserStats()->UploadLeaderboardScore(mSteamLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, Score, details.data(), static_cast<int>(details.size()));
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

			//!< ���[�_�{�[�h���o���Ă���
			mSteamLeaderboard = pCallback->m_hSteamLeaderboard;

#if 0
			//!< �w��͈�(����)�̃��[�_�[�{�[�h���擾
			const auto Handle = SteamUserStats()->DownloadLeaderboardEntries(mSteamLeaderboard, ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal, 0, 10);
#else
			//!< ���g�̃��[�_�[�{�[�h���擾
			std::vector<CSteamID> SteamIDs = { SteamUser()->GetSteamID(), /*...*/ };
			const auto Handle = SteamUserStats()->DownloadLeaderboardEntriesForUsers(mSteamLeaderboard, SteamIDs.data(), static_cast<int>(SteamIDs.size()));
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
			const auto FileName = "UGCFile";

			//!< �Ƃ肠���� uint32 0xdeadbeef ����������
			uint32 data = 0xdeadbeef;
			if (SteamRemoteStorage()->FileWrite(FileName, &data, sizeof(data))) {
				const auto Handle = SteamRemoteStorage()->FileShare(FileName);
				if (k_uAPICallInvalid != Handle) {
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

		//!< ���[�_�[�{�[�h�̃A�b�v���[�h������
		std::cout << "Leaderboard Upload Completed" << std::endl;
		
		//!< �Ƃ肠�����_�E�����[�h���J�n���Ă݂�
		DownloadLeaderboard();
	}
}

void GameClient::UploadLeaderboard()
{
	const auto Handle = SteamUserStats()->FindOrCreateLeaderboard("Test", ELeaderboardSortMethod::k_ELeaderboardSortMethodDescending, ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric);
	//const auto Handle = SteamUserStats()->FindOrCreateLeaderboard("Test", ELeaderboardSortMethod::k_ELeaderboardSortMethodAscending, ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric);
	if (k_uAPICallInvalid != Handle) {
		mLeaderboardFindResult.Set(Handle, this, &GameClient::OnLeaderboardFindResultAndUpload);
	}
}
void GameClient::DownloadLeaderboard()
{
	const auto Handle = SteamUserStats()->FindOrCreateLeaderboard("Test", ELeaderboardSortMethod::k_ELeaderboardSortMethodDescending, ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric);
	//const auto Handle = SteamUserStats()->FindOrCreateLeaderboard("Test", ELeaderboardSortMethod::k_ELeaderboardSortMethodAscending, ELeaderboardDisplayType::k_ELeaderboardDisplayTypeNumeric);
	if (k_uAPICallInvalid != Handle) {
		mLeaderboardFindResult.Set(Handle, this, &GameClient::OnLeaderboardFindResultAndDownload);
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
		ReadRemmoteStorage();
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
		int32 size;
		const auto name = SteamRemoteStorage()->GetFileNameAndSize(i, &size);
		std::cout << "[" << i << "] : " << name << "(Size = " << size << ")" << std::endl;
	}
	std::cout << std::endl;

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
void GameClient::ReadRemmoteStorage()
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

#pragma endregion