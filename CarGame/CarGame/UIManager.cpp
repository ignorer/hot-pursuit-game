#include <iostream>

#include "UIManager.h"
#include "Core/Player.h"
#include "Core/Map.h"
#include "UI/Drawing.h"
#include "UI/Map.h"
#include "UI/Car.h"
#include "UI/MainMenuWindow.h"

CUIManager::CUIManager( UI::CMainMenuWindow* _mainMenuWindow, HINSTANCE hInst ) :
	mainMenuWindow( _mainMenuWindow ),
	mapSettingsWindow( this ),
	gameResultWindow( this ),
	uiThread( std::bind( UI::CDrawing::Init, 0, nullptr ) )
{
	UI::CMapSettingsWindow::RegisterClass( hInst );
	mapSettingsWindow.Create();

	UI::CGameResultWindow::RegisterClass( hInst );
	gameResultWindow.Create();
}

int CUIManager::GetDirection( const std::vector<Core::CCoordinates>& possibleMoves, Core::CCoordinates inertia, Core::CCoordinates position ) const
{
	UI::CDrawing::DropKey();
	int key;

	UI::CDrawing::DropMouse();
	Core::CCoordinates mouse;
	while( ((key = UI::CDrawing::GetKey()) == -1) && ((mouse = UI::CDrawing::GetMouse( possibleMoves )) == Core::CCoordinates( -1, -1 )) ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}
	if( key == -1 ) {
		auto direction = mouse - inertia - position;
		key = 5 + direction.x + 3 * (-direction.y);
	}
	return key;
}

void CUIManager::InitMap( const Core::CMap& map, const std::vector<Core::CPlayer>& players, const Core::CLine& finish ) const
{
	Core::CField field = map.GetField();
	std::vector<std::vector<int>> newField;
	for( auto i = 0; i < field.size(); ++i ) {
		newField.push_back( std::vector<int>( 0 ) );
		for( auto j = 0; j < field[0].size(); ++j ) {
			newField[i].push_back( field[i][j] );
		}
	}

	std::vector<UI::CCar> cars;
	for( int i = 0; i < players.size(); ++i ) {
		auto playerCoordinates = players[i].GetPosition();
		cars.push_back( UI::CCar( UI::CCoordinates( playerCoordinates.x, playerCoordinates.y, PI / 2 ), UI::Color( i % 4 ) ) );
	}

	UI::CDrawing::InitGame( UI::CMap( newField ), cars, finish );
	UI::CDrawing::Start();
}

void CUIManager::UpdatePlayersInfo( const std::vector<Core::CPlayer>& movedPlayers ) const
{
	std::vector<int> numbers;
	std::vector<UI::CCoordinates> coordinates;
	std::vector<bool> shields;
	for( auto player : movedPlayers ) {
		numbers.push_back( player.GetNumber() );
		coordinates.push_back( UI::CCoordinates( player.GetPosition().x, player.GetPosition().y ) );
		shields.push_back( player.GetShield() > 0 );
	}
	UI::CDrawing::MoveCars( numbers, coordinates, shields );
}

void CUIManager::MarkPossibleMoves( const std::vector<Core::CCoordinates>& possibleMoves ) const
{
	UI::CDrawing::MarkPossibleMoves( possibleMoves );
}

void CUIManager::UnMarkPossibleMoves( const std::vector<Core::CCoordinates>& possibleMoves ) const
{
	UI::CDrawing::UnMarkPossibleMoves( possibleMoves );
}

void CUIManager::ShowCrashesAndRespawn( const std::set<Core::CPlayer*>& collidedPlayers ) const
{
	std::vector<int> numbers;
	std::vector<UI::CCoordinates> coordinates;
	for( auto player : collidedPlayers ) {
		numbers.push_back( player->GetNumber() );
		coordinates.push_back( UI::CCoordinates( player->GetInitialPosition().x, player->GetInitialPosition().y, PI / 2 ) );
	}
	if( !numbers.empty() ) {
		UI::CDrawing::MoveCarsToStart( numbers, coordinates );
	}
}

void CUIManager::ShowCrashes( const std::set<Core::CPlayer*>& crashedPlayers ) const
{
	std::vector<int> numbers;
	for( auto player : crashedPlayers ) {
		numbers.push_back( player->GetNumber() );
	}
	if( !numbers.empty() ) {
		UI::CDrawing::DeleteCars( numbers );
	}
}

void CUIManager::ShowPowerups( const std::map<Core::CCoordinates, PowerupType>& powerupsInfo ) const
{
	UI::CDrawing::SetPowerups( powerupsInfo );
}

void CUIManager::ShowShots( const std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>>& shots, bool needDelay ) const
{
	UI::CDrawing::SetShots( shots );
	if( needDelay ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
	}
}

void CUIManager::ShowGameResult( const std::vector<Core::CPlayer>& winners ) const
{
	gameResultWindow.SetWinners( winners );
	std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	UI::CDrawing::Stop();
	SwitchToResults();
}

void CUIManager::SwitchToMainMenu() const
{
	mainMenuWindow->MakeVisible( );
	std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	mapSettingsWindow.MakeInvisible();
	gameResultWindow.MakeInvisible();
	UI::CDrawing::HideWindow();
}

void CUIManager::SwitchToSettings() const
{
	mapSettingsWindow.MakeVisible( );
	std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	mainMenuWindow->MakeInvisible();
	gameResultWindow.MakeInvisible();
	UI::CDrawing::HideWindow();
}

void CUIManager::SwitchToGame() const
{
	UI::CDrawing::ShowWindow();
	std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	mainMenuWindow->MakeInvisible();
	mapSettingsWindow.MakeInvisible();
	gameResultWindow.MakeInvisible();
}

void CUIManager::SwitchToResults() const
{
	gameResultWindow.MakeVisible( );
	std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	mainMenuWindow->MakeInvisible();
	mapSettingsWindow.MakeInvisible();
	UI::CDrawing::HideWindow();
}

std::thread* CUIManager::GetUIThread()
{
	return &uiThread;
}

void CUIManager::FinishGame() const
{
	UI::CDrawing::DropGame();
}