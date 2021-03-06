#include "Menu.h"



Menu::Menu()
{
	//title = Label(sf::Vector2f(100, 100));
}

Menu::Menu(std::vector<std::string> pMap, std::string pMenuName, sf::Font *pFont, sf::Color pColor, sf::Vector2f pSize, int pCharacterSize, int pPadding, sf::Vector2f pPosition) {
	position = pPosition;
	buttonSize = pSize;
	sf::Vector2f buttonPosition = position;
	interButtonPadding = pPadding;
	menuSize.x = buttonSize.x;
	menuSize.y = buttonSize.y*pMap.size() + interButtonPadding * (pMap.size() - 1);

	for (int i = 0; i < pMap.size(); i++) {
		UIButton* currentUIButton;
		if (i > 0)
			buttonPosition.y += (buttonSize.y + interButtonPadding);

		currentUIButton = new UIButton(pFont, pColor, buttonPosition, buttonSize, pMap.at(i), pCharacterSize);
		currentUIButton->setName(pMap.at(i));
		childElements.push_back(currentUIButton);
	}
}

Menu::Menu(std::vector<std::pair<std::string, std::function<void()>* >> pMap, std::string pMenuName, sf::Font *pFont, sf::Color pColor, sf::Vector2f pSize, int pCharacterSize, int pPadding, sf::Vector2f pPosition){
	position = pPosition;
	buttonSize = pSize;
	sf::Vector2f buttonPosition = position;
	interButtonPadding = pPadding;
	menuSize.x = buttonSize.x;
	menuSize.y = buttonSize.y*pMap.size() + interButtonPadding * (pMap.size() - 1);

	for (int i = 0; i < pMap.size(); i++) {
		if (i > 0)
			buttonPosition.y += (buttonSize.y + interButtonPadding);

		UIButton* currentUIButton = new UIButton(pFont, pColor, buttonPosition, buttonSize, pMap.at(i).first, pCharacterSize);
		std::cout << "In menu.cpp >> [0]:" << pMap.at(i).first << " [1]:" << &pMap[i].second << std::endl;
		currentUIButton->setName(pMap.at(i).first);
		currentUIButton->click = pMap[i].second;
		childElements.push_back(currentUIButton);
	}
}

Menu::~Menu()
{
}

void Menu::build() {

}

void Menu::draw(sf::RenderWindow& pWindow)
{
	if (&childElements != nullptr) {
		for (Component* bob : childElements) {
			bob->draw(pWindow);
		}
	}
}


std::vector<Component*> Menu::getMenuButtons() {
	return childElements;
}

Component* Menu::elementAt(int i) {
	//std::cout << "Querying element at position " << i << ", nb options: " << childElements.size() << std::endl;
	if (i < childElements.size() && i > -1) {
		if ((childElements.at(i)) != nullptr) {
			return childElements.at(i);
		}
	}
	return nullptr;
}
//
//void Menu::addButton(UIButton *pButton) {
//	options.push_back(*pButton);
//}



bool Menu::isCursorHover(sf::Vector2f pCursorPosition) {
	//std::cout << "In function isCursorHover of label" << getText() << std::endl;
	bool isHover = false;
	if (pCursorPosition.x >= position.x && pCursorPosition.x <= (position.x + menuSize.x)) {
		if (pCursorPosition.y >= position.y && pCursorPosition.y <= (position.y + menuSize.y)) {
			isHover = true;
		}
	}
	return isHover;
}

int Menu::positionOf(Component *pComp) {
	if (pComp != nullptr) {
		for (int i = 0; i < childElements.size(); i++) {
			if (childElements.at(i) == pComp) {
				//std::cout << "Element found in menu at position " << i << std::endl;
				return i;
			}
		}
	}
	return -1;
}