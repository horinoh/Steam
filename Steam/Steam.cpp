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
	//!< コンソール
	setlocale(LC_ALL, "");
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	SetConsoleTitle(TEXT("Steam"));
	freopen_s(&StdOut, "CON", "w", stdout);
	freopen_s(&StdErr, "CON", "w", stderr);

	//!< 0 は無効値、正式な AppID が発行されたらここを書き換えて steam_appid.txt は削除する
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

	//!< コンソール
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
	//!< キーコード : https://msdn.microsoft.com/ja-jp/library/windows/desktop/dd375731(v=vs.85).aspx
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
		//!< ゲームサーバを作成
		if (KEY_S == wParam) {
			SteamInst->StartServer();

			//if (nullptr != ClientInst) {
			//	const auto LobbySteamID = ClientInst->GetEnteredLobbySteamID();
			//	if (LobbySteamID.IsValid()) {
			//		SteamMatchmaking()->SetLobbyData(LobbySteamID, "game_starting", "1"); // 未使用
			//	}
			//}
		}
	}
	if (nullptr != ClientInst) {
		//!< ロビー作成
		if (KEY_L == wParam) {
			ClientInst->CreateLobby();
		}
		//!< ロビー検索
		if (KEY_F == wParam) {
			ClientInst->FindLobby();
		}
		//!< ロビー参加
		if (KEY_0 <= wParam && KEY_9 >= wParam) {
			ClientInst->JoinLobby(static_cast<uint32>(wParam - KEY_0));
		}
		//!< ロビー離脱
		if (KEY_MINUS == wParam) {
			ClientInst->LeaveLobby();
		}
		//!< READY 状態のトグル
		if (KEY_R == wParam) {
			ClientInst->ToggleReady();
		}
		//!< ロビー状態の表示
		if (KEY_T == wParam) {
			ClientInst->PrintLobbyStatus(ClientInst->GetEnteredLobbySteamID());
		}
		//!< ロビーにチャットメッセージを送る
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
@brief サーバ
*/
GameServer::GameServer()
{
	//!< ポート : https://support.steampowered.com/kb_article.php?ref=8571-GLVN-8711
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

		//!< #MY_TODO 終了をクライアントへ知らせる
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
						//!< 完了したら OnValidateAuthTicketResponse() がコールバックされる
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
@brief SteamGameServer()->BeginAuthSession() のコールバックを ValidateAuthTicketResponse_t で受ける
*/
void GameServer::OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t* pCallback)
{
}

/**
@brief  のコールバックを SteamServersConnected_t で受ける
*/
void GameServer::OnSteamServersConnected(SteamServersConnected_t* pCallback)
{
	//m_bConnectedToSteam = true;
}
/**
@brief  のコールバックを SteamServerConnectFailure_t で受ける
*/
void GameServer::OnSteamServerConnectFailure(SteamServerConnectFailure_t* pCallback)
{
}
/**
@brief  のコールバックを SteamServersDisconnected_t で受ける
*/
void GameServer::OnSteamServersDisconnected(SteamServersDisconnected_t* pCallback)
{
}

/**
@brief クライアント
*/
GameClient::GameClient()
{
	if (SteamUserStats()) {
		if (!SteamUserStats()->RequestCurrentStats()) {
			std::cout << "RequestCurrentStats() failed" << std::endl;
		}
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
			if (false/*サーバが接続された*/) {
				if (SteamMatchmaking()) {
					const auto ServerSteamID = SteamGameServer()->GetSteamID();

					//!< サーバの準備ができたので接続する
					SteamMatchmaking()->SetLobbyGameServer(mEnteredLobbySteamID, 0, 0, ServerSteamID);
					
					//!< ロビーは退出する
					LeaveLobby();

					//!< サーバへクライアントが接続に入った事を伝える
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
		//!< タイム・アウトしたらする処理
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
@brief のコールバックを SteamServersConnected_t で受ける
*/
void GameClient::OnSteamServersConnected(SteamServersConnected_t* pCallback)
{
}
/**
@brief のコールバックを SteamServersDisconnected_t で受ける
*/
void GameClient::OnSteamServersDisconnected(SteamServersDisconnected_t* pCallback)
{
}
/**
@brief のコールバックを SteamServerConnectFailure_t で受ける
*/
void GameClient::OnSteamServerConnectFailure(SteamServerConnectFailure_t* pCallback)
{
}

/**
@brief SteamMatchmaking()->CreateLobby() のコールバックを LobbyCreated_t で受ける
*/
void GameClient::OnLobbyCreated(LobbyCreated_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure) {
		//!< 既にロビーに入っているなら抜ける
		LeaveLobby();

		mEnteredLobbySteamID = pCallback->m_ulSteamIDLobby;
		if (SteamUser() && SteamMatchmaking()) {
			mState = EClientState::InLobby;

			//!< 検索フィルタ用に、ゲーム名を設定しておく ["game"] = "SpaceWar"
			SteamMatchmaking()->SetLobbyData(mEnteredLobbySteamID, "game", "SpaceWar");

			//SteamMatchmaking()->SetLobbyData(mEnteredLobbySteamID, "name", (std::to_string(SteamUser()->GetSteamID().ConvertToUint64()) + "'s LOBBY").c_str());
			SteamMatchmaking()->SetLobbyData(mEnteredLobbySteamID, "name", (std::string(SteamFriends()->GetPersonaName()) + "'s LOBBY").c_str());

			std::cout << "Created : " << mEnteredLobbySteamID.ConvertToUint64() << " (" << SteamMatchmaking()->GetLobbyData(mEnteredLobbySteamID, "name")  << ")" << std::endl;

			PrintLobbyStatus(mEnteredLobbySteamID);
		}
	}
}
/**
@brief SteamMatchmaking()->RequestLobbyList() のコールバックを LobbyMatchList_t で受ける
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
				
				//!< 自分の作成したロビーは選択肢に含めない
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
@brief SteamMatchmaking()->JoinLobby() のコールバックを LobbyEnter_t で受ける
*/
void GameClient::OnLobbyEntered(LobbyEnter_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure) {
		//!< 既にロビーに入っているなら抜ける
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
@brief SteamMatchmaking()->SetLobbyGameServer() のコールバックを LobbyGameCreated_t で受ける
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
@brief SteamMatchmaking()->SendLobbyChatMsg() のコールバックを LobbyChatMsg_t で受ける
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
		//!< ["game"] = "SpaceWar" のものだけフィルタリングする場合
		//SteamMatchmaking()->AddRequestLobbyListStringFilter("game", "SpaceWar", k_ELobbyComparisonEqual);

		//!< (ここでは) 見つけるのは 10 個までにしておく
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
@brief Spacewar
	ユーザスタッツ
	"NumGames", "NumWins", "NumLosses", "FeetTraveled", "AverageSpeed", "Unused2", "MaxFeetTraveled"
	実績
	"ACH_WIN_ONE_GAME", "ACH_WIN_100_GAMES", "ACH_TRAVEL_FAR_ACCUM", "ACH_TRAVEL_FAR_SINGLE", "NEW_ACHIEVEMENT_0_4",
@note Spacewar にどんな ユーザスタッツ、実績 があるかはここで確認できる https://steamdb.info/app/480/stats/
*/
void GameClient::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	if (k_EResultOK == pCallback->m_eResult) {
		if (SteamUserStats()) {
			std::cout << "UserStatsReceived. GameID = " << pCallback->m_nGameID << ", UserName = " << SteamFriends()->GetFriendPersonaName(pCallback->m_steamIDUser) << std::endl;

			//!< スタッツ
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

			//!< 実績
			std::cout << "Achievements" << std::endl;
			bool bUnlocked;
			const auto Count = SteamUserStats()->GetNumAchievements();
			for (uint32 i = 0; i < Count; ++i) {
				const auto Name = SteamUserStats()->GetAchievementName(i);
				if (SteamUserStats()->GetAchievement(Name, &bUnlocked)) {
					std::cout << "\t" << Name << " : " << (bUnlocked ? "Unlocked" : "Locked") << std::endl;
					std::cout << "\t\t" << SteamUserStats()->GetAchievementDisplayAttribute(Name, "name") << std::endl;
					std::cout << "\t\t" << SteamUserStats()->GetAchievementDisplayAttribute(Name, "desc") << std::endl;

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
	//std::cout << pCallback->m_bGroupAchievement << std::endl; //!< グループ実績かどうか
}
void GameClient::StoreStats()
{
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
		//!< 1マイル = 5280フィート
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

		//mNumGames = mNumWins = mNumLosses = 0;
		//SteamUserStats()->SetStat("NumGames", mNumGames);
		//SteamUserStats()->SetStat("NumWins", mNumWins);
		//SteamUserStats()->SetStat("NumLosses", mNumLosses);

		//mFeetTraveled = mMaxFeetTraveled = 0.0f;
		//SteamUserStats()->SetStat("FeetTraveled", mFeetTraveled);
		//SteamUserStats()->SetStat("MaxFeetTraveled", mMaxFeetTraveled);
		//SteamUserStats()->UpdateAvgRateStat("AverageSpeed", 0.0f, 0.0);

		SteamUserStats()->StoreStats();
	}
}

#pragma endregion