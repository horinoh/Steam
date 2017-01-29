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
	std::cout << "t: Lobby Status" << std::endl;
	std::cout << "r: Toggle READY" << std::endl;
	std::cout << "s: Start Game" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
}
void Steam::HandleInput(WPARAM wParam)
{
	//!< キーコード : https://msdn.microsoft.com/ja-jp/library/windows/desktop/dd375731(v=vs.85).aspx
	static const WPARAM KEY_S = 0x53;
	static const WPARAM KEY_L = 0x4c;
	static const WPARAM KEY_F = 0x46;
	static const WPARAM KEY_0 = 0x30;
	static const WPARAM KEY_9 = 0x39;
	static const WPARAM KEY_R = 0x52;
	static const WPARAM KEY_T = 0x54;
	static const WPARAM KEY_U = 0x55;

	if (KEY_U == wParam) {
		PrintUsage();
	}
	const auto ServerInst = SteamInst->GetGameServer();
	if (nullptr == ServerInst) {
		//!< (ゲームサーバを作成して)ゲームスタート
		if (KEY_S == wParam) {
			SteamInst->StartServer();
		}
	}
	const auto ClientInst = GetGameClient();
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
		//!< READY 状態のトグル
		if (KEY_R == wParam) {
			ClientInst->ToggleReady();
		}
		//!< ロビー状態の表示
		if (KEY_T == wParam) {
			ClientInst->LobbyStatus(ClientInst->GetEnteredLobbySteamID());
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

}

void GameServer::OnSteamServersConnected(SteamServersConnected_t* pCallback)
{
	//m_bConnectedToSteam = true;
}
void GameServer::OnSteamServerConnectFailure(SteamServerConnectFailure_t* pCallback)
{
}
void GameServer::OnSteamServersDisconnected(SteamServersDisconnected_t* pCallback)
{
}

/**
@brief クライアント
*/
GameClient::GameClient()
{
}
GameClient::~GameClient()
{
	if (SteamMatchmaking()) {
		if (k_HAuthTicketInvalid != mAuthTicket) {
			if (SteamUser()) {
				SteamUser()->CancelAuthTicket(mAuthTicket);
			}
			mAuthTicket = k_HAuthTicketInvalid;
		}

		if (mCreatedLobbySteamID.IsValid()) {
			SteamMatchmaking()->LeaveLobby(mCreatedLobbySteamID);
			mCreatedLobbySteamID = CSteamID();
		}
		LeaveLobby();
	}
}

void GameClient::OnTimer(HWND hWnd, HINSTANCE hInstance)
{
	SteamAPI_RunCallbacks();

	//!< ロビーに入った
	if (mEnteredLobbySteamID.IsValid()) {
		//!< サーバが存在する
		if (SteamGameServer()) {
			const auto ServerSteamID = SteamGameServer()->GetSteamID();
			if (ServerSteamID.IsValid()) {
				//!< サーバに入る (サーバとロビーを関連付ける)
				SteamMatchmaking()->SetLobbyGameServer(mEnteredLobbySteamID, 0, 0, ServerSteamID);

				if (SteamUser()) {
					char Token[1024];
					uint32 TokenLength = 0;
					mAuthTicket = SteamUser()->GetAuthSessionTicket(Token, sizeof(Token), &TokenLength);
					// #MY_TODO サーバへ伝える

					// #MY_TODO サーバ
					//SteamGameServer()->BeginAuthSession() をコール、ValidateAuthTicketResponse_t でコールバックされる
				}
			}
		}
	}
}

void GameClient::OnLobbyCreated(LobbyCreated_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure) {
		mEnteredLobbySteamID = mCreatedLobbySteamID = pCallback->m_ulSteamIDLobby;
		if (SteamUser() && SteamMatchmaking()) {
			const auto SteamID = SteamUser()->GetSteamID();

			//SteamMatchmaking()->SetLobbyData(mCreatedLobbySteamID, "name", (std::to_string(SteamID.ConvertToUint64()) + "'s LOBBY").c_str());
			SteamMatchmaking()->SetLobbyData(mCreatedLobbySteamID, "name", "MY LOBBY");

			std::cout << "Created : " << SteamMatchmaking()->GetLobbyData(mCreatedLobbySteamID, "name") << std::endl;
			LobbyStatus(mEnteredLobbySteamID);
		}
	}
}
void GameClient::OnLobbyMatchListed(LobbyMatchList_t* pCallback, bool bIOFailure)
{
	std::cout << "---- Found Lobby Match List -------------------" << std::endl;
	mFoundLoobySteamID.clear();
	if (!bIOFailure) {
		if (SteamMatchmaking() && SteamUser()) {
			const auto MySteamID = SteamUser()->GetSteamID();

			const auto Count = pCallback->m_nLobbiesMatching;
			for (uint32 i = 0; i < Count; ++i) {
				const auto LobbySteamID = SteamMatchmaking()->GetLobbyByIndex(i);
				
				//!< 自分の作成したロビーは選択肢に入れない
				if (MySteamID != SteamMatchmaking()->GetLobbyOwner(LobbySteamID)) {
					std::cout << "[" << mFoundLoobySteamID.size() << "] : " << SteamMatchmaking()->GetLobbyData(LobbySteamID, "name") << std::endl;
					mFoundLoobySteamID.push_back(LobbySteamID);
				}
				else {
					std::cout << "[-] : " << SteamMatchmaking()->GetLobbyData(LobbySteamID, "name") << std::endl;
				}
			}
		}
	}
	std::cout << "-----------------------------------------------" << std::endl;
}
void GameClient::OnLobbyEntered(LobbyEnter_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure) {
		if (SteamMatchmaking()) {
			if (!mEnteredLobbySteamID.IsValid()) {
				mEnteredLobbySteamID = pCallback->m_ulSteamIDLobby;

				std::cout << "Entered : " << SteamMatchmaking()->GetLobbyData(mEnteredLobbySteamID, "name") << std::endl;

				LobbyStatus(mEnteredLobbySteamID);
			}
		}
	}
}

void GameClient::CreateLobby()
{
	if (SteamMatchmaking()) {
		if (!mOnLobbyCreated.IsActive()) {
			const auto Handle = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 4);
			if (k_uAPICallInvalid != Handle) {
				mOnLobbyCreated.Set(Handle, this, &GameClient::OnLobbyCreated);
			}
		}
	}
}
void GameClient::FindLobby()
{
	if (SteamMatchmaking()) {
		const auto Handle = SteamMatchmaking()->RequestLobbyList();
		if (k_uAPICallInvalid != Handle) {
			mOnLobbyMatchListed.Set(Handle, this, &GameClient::OnLobbyMatchListed);
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
			}
		}
	}
}

void GameClient::LeaveLobby()
{
	if (mEnteredLobbySteamID.IsValid()) {
		SteamMatchmaking()->LeaveLobby(mEnteredLobbySteamID);
		mEnteredLobbySteamID = CSteamID();
	}
}

void GameClient::LobbyStatus(const CSteamID LobbySteamID)
{
	if (SteamMatchmaking()) {
		if (LobbySteamID.IsValid()) {
			std::cout << "---- " << SteamMatchmaking()->GetLobbyData(LobbySteamID, "name") << " ----------------" << std::endl;

			const auto Count = SteamMatchmaking()->GetNumLobbyMembers(LobbySteamID);
			for (auto i = 0; i < Count; ++i) {
				const auto MemberSteamID = SteamMatchmaking()->GetLobbyMemberByIndex(LobbySteamID, i);

				const auto ReadyState = std::atoi(SteamMatchmaking()->GetLobbyMemberData(LobbySteamID, MemberSteamID, "ready"));
				//std::cout << MemberSteamID.ConvertToUint64() << (1 == ReadyState ? " (READY) " : "") << std::endl;
				std::cout << (SteamUser()->GetSteamID() == MemberSteamID ? "ME" : "SOMEONE") << (1 == ReadyState ? " (READY) " : "") << std::endl;
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

				LobbyStatus(LobbySteamID);
			}
		}
	}
}
#pragma endregion