#pragma once

#include "resource.h"

#include <iostream>
#include <string>
#include <vector>

#pragma warning(disable:4819)
#pragma warning(disable:4996)
#include "steam/steam_api.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if(nullptr != x) { delete x; x = nullptr; }
#endif
#ifndef SAFE_FCLOSE
#define SAFE_FCLOSE(x) if(nullptr != x) { fclose(x); x = nullptr; }

#endif
class Steam
{
public:
	Steam();
	virtual ~Steam();

	virtual void OnTimer(HWND hWnd, HINSTANCE hInstance);

	//void StartServer(){}
	void StartClient();

	//class GameServer* GetGameServer() { return mpGameServer; }
	class GameClient* GetGameClient() { return mpGameClient; }

	static void PrintUsage();

protected:
	FILE* StdOut = nullptr;
	FILE* StdErr = nullptr;

	//class GameServer* mpGameServer = nullptr;
	class GameClient* mpGameClient = nullptr;
};

class GameClient
{
public:
	GameClient();
	virtual ~GameClient();

	virtual void OnTimer(HWND hWnd, HINSTANCE hInstance);

	void OnLobbyCreated(LobbyCreated_t *pCallback, bool bIOFailure);
	void OnLobbyMatchListed(LobbyMatchList_t* pCallback, bool bIOFailure);
	void OnLobbyEntered(LobbyEnter_t *pCallback, bool bIOFailure);

	void CreateLobby();
	void FindLobby();
	void JoinLobby(const uint32 Index);
	void LeaveLobby();

	static void LobbyStatus(const CSteamID LobbySteamID);
	void ToggleReady();

	CSteamID GetCreatedLobbySteamID() const { return mCreatedLobbySteamID; }
	CSteamID GetEnteredLobbySteamID() const { return mEnteredLobbySteamID; }

protected:
	CCallResult<GameClient, LobbyCreated_t> mOnLobbyCreated;
	CCallResult<GameClient, LobbyMatchList_t> mOnLobbyMatchListed;
	CCallResult<GameClient, LobbyEnter_t> mOnLobbyEntered;

	CSteamID mCreatedLobbySteamID;
	CSteamID mEnteredLobbySteamID;
	std::vector<CSteamID> mFoundLoobySteamID;
};