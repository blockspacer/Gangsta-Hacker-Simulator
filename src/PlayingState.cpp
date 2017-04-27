#include "PlayingState.h"


void ClrScrn(char attrib) {
	const int ROWS = 50;
	const int COLS = 80;
	COORD pos = { 0, 0 };
	DWORD written;
	unsigned size;
	CHAR_INFO disp[ROWS][COLS];
	HANDLE console;
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	//COORD size = { COLS, ROWS };
	COORD src = { 0, 0 };
	SMALL_RECT  dest = { 0, 0, COLS, ROWS };

	size = ROWS * COLS;

	FillConsoleOutputCharacter(console, ' ', size, pos, &written);
	FillConsoleOutputAttribute(console, attrib, size, pos, &written);
	SetConsoleCursorPosition(console, pos);
}


PlayingState::PlayingState()
{
	system("cls");
	std::cout << std::endl << "\tCreating new game..." << std::endl;
	playerCompanies.push_back(createCompany());
	std::cout << "\tCompany created" << std::endl;
	playerCompanies.at(0)->createTechTree();


	running = true;
	quit = false;

	system("cls");

	auto start = std::chrono::steady_clock::now();
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start); //Why not as an object attribute
																					   //Or multipliate the timer for every action (code, research,...) by a speed coef -> but wouldn't speed up animations ?

	testMissions();
}


PlayingState::~PlayingState()
{
	for (Company* cmp : playerCompanies) {
		delete(cmp);
	}
	for (Company* cmp : companies) {
		delete(cmp);
	}
	for (Mission* m : missions) {
		delete(m);
	}
}

void PlayingState::init() {

}
void PlayingState::cleanUp() {

}

void PlayingState::pause() {

}
void PlayingState::resume() {

}

void PlayingState::handleEvents() {

}
GameState* PlayingState::update(std::chrono::microseconds elapsed) {
	//system("cls");
	ClrScrn(0x71);
	//std::cout << "At pos 1 elapsed = " << elapsed.count() << std::endl;
	updateActivities(elapsed);
	//displayMissionsOfCharacter(playerCompanies.at(0).getCharacter(0));
	playerCompanies.at(0)->getCharacter(0)->displayStatistics();
	return nullptr;
}
void PlayingState::draw() {

}

Company* PlayingState::createCompany() {
	std::string companyName;
	std::cout << "\tWell, we are going to create your Company, what name did you think to ? ";
	std::cin >> companyName;
	Company* newCompany = new Company(companyName);

	return newCompany;
}

void PlayingState::testMissions() {

	//Create Characters and companies
	Character badGuysDirector("Brandon", Male, false);
	Character goodGuysDirector("Michelle", Female, false);
	Company *goodGuys = new Company("ThegoodGuys", 1, 0);
	goodGuys->addCharacter(&goodGuysDirector);

	//Create Missions and set people giving these missions
	Mission attack("Hack bob", "Hi, we need to break these guys, we'll pay you a lot if you hack X company", std::make_shared<Company>(*goodGuys), 0, 0, 0, 100);
	Mission defend("Protect bob", "X company has reasons to believe that someone is trying to hack them, they ask for help", std::make_shared<Company>(*goodGuys), 1, 100, 0, 0);
	attack.setApplicant(&badGuysDirector);
	defend.setApplicant(&goodGuysDirector);

	// Affect these missions
	Company *mainCompany = playerCompanies.at(0);
	if (mainCompany->getNbCharacters() != 0)
		if (mainCompany->getCharacter(0) != nullptr) {

			attack.assignToCompany(mainCompany);
			defend.assignToCompany(mainCompany);
			attack.assignToCharacter(mainCompany->getCharacter(0));
			defend.assignToCharacter(mainCompany->getCharacter(0));
			missions.push_back(&attack);
			missions.push_back(&defend);
			setCurrentMissionOf(mainCompany->getCharacter(0), &attack);
			//displayMissionsOfCharacter(mainCompany->getCharacter(0));
			//system("pause");
		}
		else {
			std::cout << "Mission could not be affected to character";
			system("pause");
			exit(1);
		}
	else {
		//std::cout << "There are " << mainCompany->getNbCharacters() << "characters in the comany" << std::endl;
	}


#pragma region testsActivities
	Activity actTest;
	mainCompany->getCharacter(0)->addPossibleActivity(&actTest);
	mainCompany->getCharacter(0)->setCurrentActivity(0);
	mainCompany->getCharacter(0)->getCurrentActivity()->start();
	//std::cout << "Time elapsed (just started): " << mainCompany->getCharacter(0)->getCurrentActivity()->getTimeElapsed().count() << std::endl;
	//std::cout << "Current activity in .. is : " << mainCompany->getCharacter(0)->getCurrentActivityIndex() << std::endl;
#pragma endregion
}


void PlayingState::setCurrentMissionOf(Character *pChar, Mission *pMission) {
	Mission *old = getCurrentMissionOf(pChar);
	if (old != nullptr)
		old->setCurrent(false);

	pMission->setCurrent(true);
}

Mission* PlayingState::getCurrentMissionOf(Character *pChar) {
	Mission* current = nullptr;
	for (int i = 0; i < missions.size(); i++) {
		if (missions.at(i)->getCharacterAssigned() == pChar && missions.at(i)->isCurrent()) {
			current = missions.at(i);
			//std::cout << "Current mission of " << pChar->getName() << " found!" << std::endl;
		}
	}
	return current;
}

void PlayingState::updateActivities(std::chrono::microseconds elapsed) {
	for (Company* cmp : playerCompanies) {
		cmp->updateActivities(elapsed);
	}
}


void PlayingState::displayMissionsOfCharacter(Character *pChar) {
	Mission *current = getCurrentMissionOf(pChar);
	if (current != NULL) {
		std::cout << "Current mission : " << std::endl;
		std::cout << "\t";
		current->headLines();
	}
	if (getNbOfMissionsFor(pChar) > 1) {
		std::cout << "Other missions : " << std::endl;
		for (Mission* mission : missions) {
			if (mission->isAssignedTo(pChar) && mission != current && !(mission->isFinished())) {
				std::cout << "\t";
				mission->headLines();
			}
		}
	}
	else
		std::cout << "No other missions are assigned to " << pChar->getName() << std::endl;
}

int PlayingState::getNbOfMissionsFor(Character *pChar) {
	int nbMissions = 0;
	for (Mission* mission : missions) {
		if (mission->isAssignedTo(pChar))
			nbMissions++;
	}
	return nbMissions;
}