/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2017 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Container.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Clipping.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    static std::map<std::string, ObjectConverter> defaultRendererValues =
    {
        {"borders", Borders{2}},
        {"padding", Padding{2, 0, 0, 0}},
        {"bordercolor", sf::Color::Black},
        {"textcolor", sf::Color::Black},
        {"backgroundcolor", Color{245, 245, 245}},
        {"arrowcolor", Color{60, 60, 60}},
        {"arrowcolorhover", sf::Color::Black},
        {"arrowbackgroundcolor", Color{245, 245, 245}},
        {"arrowbackgroundcolorhover", sf::Color::White}
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::ComboBox()
    {
        m_callback.widgetType = "ComboBox";
        m_type = "ComboBox";

        m_draggableWidget = true;

        addSignal<sf::String, TypeSet<sf::String, sf::String>>("ItemSelected");

        initListBox();

        m_renderer = aurora::makeCopied<ComboBoxRenderer>();
        setRenderer(RendererData::create(defaultRendererValues));

        setSize({150, 24});
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::ComboBox(const ComboBox& other) :
        Widget {other},
        m_nrOfItemsToDisplay {other.m_nrOfItemsToDisplay},
        m_listBox {ListBox::copy(other.m_listBox)},
        m_text {other.m_text},
        m_spriteBackground {other.m_spriteBackground},
        m_spriteArrowUp {other.m_spriteArrowUp},
        m_spriteArrowDown {other.m_spriteArrowDown},
        m_spriteArrowUpHover {other.m_spriteArrowUpHover},
        m_spriteArrowDownHover {other.m_spriteArrowDownHover},
        m_bordersCached {other.m_bordersCached},
        m_paddingCached {other.m_paddingCached},
        m_borderColorCached {other.m_borderColorCached},
        m_backgroundColorCached {other.m_backgroundColorCached},
        m_arrowColorCached {other.m_arrowColorCached},
        m_arrowColorHoverCached {other.m_arrowColorHoverCached},
        m_arrowBackgroundColorCached {other.m_arrowBackgroundColorCached},
        m_arrowBackgroundColorHoverCached{other.m_arrowBackgroundColorHoverCached}
    {
        if (m_listBox != nullptr)
        {
            m_listBox->disconnectAll();
            initListBox();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::ComboBox(ComboBox&& other) :
        Widget {std::move(other)},
        m_nrOfItemsToDisplay {std::move(other.m_nrOfItemsToDisplay)},
        m_listBox {std::move(other.m_listBox)},
        m_text {std::move(other.m_text)},
        m_spriteBackground {std::move(other.m_spriteBackground)},
        m_spriteArrowUp {std::move(other.m_spriteArrowUp)},
        m_spriteArrowDown {std::move(other.m_spriteArrowDown)},
        m_spriteArrowUpHover {std::move(other.m_spriteArrowUpHover)},
        m_spriteArrowDownHover {std::move(other.m_spriteArrowDownHover)},
        m_bordersCached {std::move(other.m_bordersCached)},
        m_paddingCached {std::move(other.m_paddingCached)},
        m_borderColorCached {std::move(other.m_borderColorCached)},
        m_backgroundColorCached {std::move(other.m_backgroundColorCached)},
        m_arrowColorCached {std::move(other.m_arrowColorCached)},
        m_arrowColorHoverCached {std::move(other.m_arrowColorHoverCached)},
        m_arrowBackgroundColorCached {std::move(other.m_arrowBackgroundColorCached)},
        m_arrowBackgroundColorHoverCached{std::move(other.m_arrowBackgroundColorHoverCached)}
    {
        if (m_listBox != nullptr)
        {
            m_listBox->disconnectAll();
            initListBox();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox& ComboBox::operator=(const ComboBox& other)
    {
        if (this != &other)
        {
            ComboBox temp{other};
            Widget::operator=(other);

            std::swap(m_nrOfItemsToDisplay, temp.m_nrOfItemsToDisplay);
            swap(m_listBox, temp.m_listBox);
            std::swap(m_text, temp.m_text);
            std::swap(m_spriteBackground, temp.m_spriteBackground);
            std::swap(m_spriteArrowUp, temp.m_spriteArrowUp);
            std::swap(m_spriteArrowDown, temp.m_spriteArrowDown);
            std::swap(m_spriteArrowUpHover, temp.m_spriteArrowUpHover);
            std::swap(m_spriteArrowDownHover, temp.m_spriteArrowDownHover);
            std::swap(m_bordersCached, temp.m_bordersCached);
            std::swap(m_paddingCached, temp.m_paddingCached);
            std::swap(m_borderColorCached, temp.m_borderColorCached);
            std::swap(m_backgroundColorCached, temp.m_backgroundColorCached);
            std::swap(m_arrowColorCached, temp.m_arrowColorCached);
            std::swap(m_arrowColorHoverCached, temp.m_arrowColorHoverCached);
            std::swap(m_arrowBackgroundColorCached, temp.m_arrowBackgroundColorCached);
            std::swap(m_arrowBackgroundColorHoverCached, temp.m_arrowBackgroundColorHoverCached);
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox& ComboBox::operator=(ComboBox&& other)
    {
        if (this != &other)
        {
            Widget::operator=(std::move(other));
            std::swap(m_nrOfItemsToDisplay, other.m_nrOfItemsToDisplay);
            swap(m_listBox, other.m_listBox);
            std::swap(m_text, other.m_text);
            std::swap(m_spriteBackground, other.m_spriteBackground);
            std::swap(m_spriteArrowUp, other.m_spriteArrowUp);
            std::swap(m_spriteArrowDown, other.m_spriteArrowDown);
            std::swap(m_spriteArrowUpHover, other.m_spriteArrowUpHover);
            std::swap(m_spriteArrowDownHover, other.m_spriteArrowDownHover);
            std::swap(m_bordersCached, other.m_bordersCached);
            std::swap(m_paddingCached, other.m_paddingCached);
            std::swap(m_borderColorCached, other.m_borderColorCached);
            std::swap(m_backgroundColorCached, other.m_backgroundColorCached);
            std::swap(m_arrowColorCached, other.m_arrowColorCached);
            std::swap(m_arrowColorHoverCached, other.m_arrowColorHoverCached);
            std::swap(m_arrowBackgroundColorCached, other.m_arrowBackgroundColorCached);
            std::swap(m_arrowBackgroundColorHoverCached, other.m_arrowBackgroundColorHoverCached);
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::Ptr ComboBox::create()
    {
        return std::make_shared<ComboBox>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::Ptr ComboBox::copy(ConstPtr comboBox)
    {
        if (comboBox)
            return std::static_pointer_cast<ComboBox>(comboBox->clone());
        return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        m_spriteBackground.setSize(getInnerSize());

        float height = getInnerSize().y - m_paddingCached.top - m_paddingCached.bottom;

        if (height > 0)
        {
            m_listBox->setItemHeight(static_cast<unsigned int>(height));
            updateListBoxHeight();
        }

        if (m_spriteArrowUp.isSet() && m_spriteArrowDown.isSet())
        {
            m_spriteArrowUp.setSize({m_spriteArrowUp.getTexture().getImageSize().x * (height / m_spriteArrowUp.getTexture().getImageSize().y), height});
            m_spriteArrowDown.setSize({m_spriteArrowDown.getTexture().getImageSize().x * (height / m_spriteArrowDown.getTexture().getImageSize().y), height});

            m_spriteArrowUpHover.setSize(m_spriteArrowUp.getSize());
            m_spriteArrowDownHover.setSize(m_spriteArrowDown.getSize());
        }

        m_text.setCharacterSize(m_listBox->getTextSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setItemsToDisplay(size_t nrOfItemsInList)
    {
        m_nrOfItemsToDisplay = nrOfItemsInList;

        if (m_nrOfItemsToDisplay < m_listBox->getItemCount())
            updateListBoxHeight();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t ComboBox::getItemCount() const
    {
        return m_listBox->getItemCount();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t ComboBox::getItemsToDisplay() const
    {
        return m_nrOfItemsToDisplay;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::addItem(const sf::String& item, const sf::String& id)
    {
        // Make room to add another item, until there are enough items
        if ((m_nrOfItemsToDisplay == 0) || (m_listBox->getItemCount() < m_nrOfItemsToDisplay))
            updateListBoxHeight();

        return m_listBox->addItem(item, id);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::setSelectedItem(const sf::String& itemName)
    {
        bool ret = m_listBox->setSelectedItem(itemName);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::setSelectedItemById(const sf::String& id)
    {
        bool ret = m_listBox->setSelectedItemById(id);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::setSelectedItemByIndex(size_t index)
    {
        bool ret = m_listBox->setSelectedItemByIndex(index);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::deselectItem()
    {
        m_text.setString("");
        m_listBox->deselectItem();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::removeItem(const sf::String& itemName)
    {
        // Implemented like removeItemByIndex instead of like removeItemById because m_listBox->getItems() may be slow

        bool ret = m_listBox->removeItem(itemName);

        m_text.setString(m_listBox->getSelectedItem());

        // Shrink the list size
        if ((m_nrOfItemsToDisplay == 0) || (m_listBox->getItemCount() < m_nrOfItemsToDisplay))
            updateListBoxHeight();

        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::removeItemById(const sf::String& id)
    {
        const auto& ids = m_listBox->getItemIds();
        for (size_t i = 0; i < ids.size(); ++i)
        {
            if (ids[i] == id)
                return removeItemByIndex(i);
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::removeItemByIndex(size_t index)
    {
        bool ret = m_listBox->removeItemByIndex(index);

        m_text.setString(m_listBox->getSelectedItem());

        // Shrink the list size
        if ((m_nrOfItemsToDisplay == 0) || (m_listBox->getItemCount() < m_nrOfItemsToDisplay))
            updateListBoxHeight();

        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::removeAllItems()
    {
        m_text.setString("");
        m_listBox->removeAllItems();

        updateListBoxHeight();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String ComboBox::getItemById(const sf::String& id) const
    {
        return m_listBox->getItemById(id);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String ComboBox::getSelectedItem() const
    {
        return m_listBox->getSelectedItem();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String ComboBox::getSelectedItemId() const
    {
        return m_listBox->getSelectedItemId();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int ComboBox::getSelectedItemIndex() const
    {
        return m_listBox->getSelectedItemIndex();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::changeItem(const sf::String& originalValue, const sf::String& newValue)
    {
        bool ret = m_listBox->changeItem(originalValue, newValue);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::changeItemById(const sf::String& id, const sf::String& newValue)
    {
        bool ret = m_listBox->changeItemById(id, newValue);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::changeItemByIndex(size_t index, const sf::String& newValue)
    {
        bool ret = m_listBox->changeItemByIndex(index, newValue);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<sf::String> ComboBox::getItems()
    {
        return m_listBox->getItems();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::vector<sf::String>& ComboBox::getItemIds()
    {
        return m_listBox->getItemIds();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setMaximumItems(size_t maximumItems)
    {
        m_listBox->setMaximumItems(maximumItems);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t ComboBox::getMaximumItems() const
    {
        return m_listBox->getMaximumItems();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setTextSize(unsigned int textSize)
    {
        m_listBox->setTextSize(textSize);
        m_text.setCharacterSize(m_listBox->getTextSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int ComboBox::getTextSize() const
    {
        return m_listBox->getTextSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setParent(Container* parent)
    {
        hideListBox();
        Widget::setParent(parent);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::mouseOnWidget(sf::Vector2f pos) const
    {
        return sf::FloatRect{0, 0, getSize().x, getSize().y}.contains(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::leftMousePressed(sf::Vector2f)
    {
        m_mouseDown = true;

        // If the list wasn't visible then open it
        if (!m_listBox->isVisible())
        {
            // Show the list
            showListBox();

            // Reselect the selected item to make sure it is always among the visible items when the list opens
            if (m_listBox->getSelectedItemIndex() >= 0)
                m_listBox->setSelectedItemByIndex(m_listBox->getSelectedItemIndex());
        }
        else // This list was already open, so close it now
            hideListBox();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::mouseWheelScrolled(float delta, int, int)
    {
        // Only act to scrolling when the list is not being shown
        if (!m_listBox->isVisible())
        {
            // Check if you are scrolling down
            if (delta < 0)
            {
                // Select the next item
                if (static_cast<size_t>(m_listBox->getSelectedItemIndex() + 1) < m_listBox->getItemCount())
                {
                    m_listBox->setSelectedItemByIndex(static_cast<size_t>(m_listBox->getSelectedItemIndex() + 1));
                    m_text.setString(m_listBox->getSelectedItem());
                }
            }
            else // You are scrolling up
            {
                // Select the previous item
                if (m_listBox->getSelectedItemIndex() > 0)
                {
                    m_listBox->setSelectedItemByIndex(static_cast<size_t>(m_listBox->getSelectedItemIndex() - 1));
                    m_text.setString(m_listBox->getSelectedItem());
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::rendererChanged(const std::string& property)
    {
        if (property == "borders")
        {
            m_bordersCached = getRenderer()->getBorders();
            updateSize();
        }
        else if (property == "padding")
        {
            m_paddingCached = getRenderer()->getPadding();
            updateSize();
        }
        else if (property == "textcolor")
        {
            m_text.setColor(getRenderer()->getTextColor());
        }
        else if (property == "textstyle")
        {
            m_text.setStyle(getRenderer()->getTextStyle());
        }
        else if (property == "texturebackground")
        {
            m_spriteBackground.setTexture(getRenderer()->getTextureBackground());
        }
        else if (property == "texturearrowup")
        {
            m_spriteArrowUp.setTexture(getRenderer()->getTextureArrowUp());
            updateSize();
        }
        else if (property == "texturearrowuphover")
        {
            m_spriteArrowUpHover.setTexture(getRenderer()->getTextureArrowUpHover());
        }
        else if (property == "texturearrowdown")
        {
            m_spriteArrowDown.setTexture(getRenderer()->getTextureArrowDown());
            updateSize();
        }
        else if (property == "texturearrowdownhover")
        {
            m_spriteArrowDownHover.setTexture(getRenderer()->getTextureArrowDownHover());
        }
        else if (property == "listbox")
        {
            m_listBox->setRenderer(getRenderer()->getListBox());
        }
        else if (property == "bordercolor")
        {
            m_borderColorCached = getRenderer()->getBorderColor();
        }
        else if (property == "backgroundcolor")
        {
            m_backgroundColorCached = getRenderer()->getBackgroundColor();
        }
        else if (property == "arrowbackgroundcolor")
        {
            m_arrowBackgroundColorCached = getRenderer()->getArrowBackgroundColor();
        }
        else if (property == "arrowbackgroundcolorhover")
        {
            m_arrowBackgroundColorHoverCached = getRenderer()->getArrowBackgroundColorHover();
        }
        else if (property == "arrowcolor")
        {
            m_arrowColorCached = getRenderer()->getArrowColor();
        }
        else if (property == "arrowcolorhover")
        {
            m_arrowColorHoverCached = getRenderer()->getArrowColorHover();
        }
        else if (property == "opacity")
        {
            Widget::rendererChanged(property);

            m_spriteBackground.setOpacity(m_opacityCached);
            m_spriteArrowUp.setOpacity(m_opacityCached);
            m_spriteArrowUpHover.setOpacity(m_opacityCached);
            m_spriteArrowDown.setOpacity(m_opacityCached);
            m_spriteArrowDownHover.setOpacity(m_opacityCached);

            m_text.setOpacity(m_opacityCached);
        }
        else if (property == "font")
        {
            Widget::rendererChanged(property);

            m_text.setFont(m_fontCached);

            if (m_listBox->getRenderer()->getFont() == nullptr)
                m_listBox->getRenderer()->setFont(m_fontCached);

            updateSize();
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Vector2f ComboBox::getInnerSize() const
    {
        return {getSize().x - m_bordersCached.left - m_bordersCached.right, getSize().y - m_bordersCached.top - m_bordersCached.bottom};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::updateListBoxHeight()
    {
        Borders borders = m_listBox->getRenderer()->getBorders();
        Padding padding = m_listBox->getRenderer()->getPadding();

        if (m_nrOfItemsToDisplay > 0)
            m_listBox->setSize({getSize().x, (m_listBox->getItemHeight() * (std::min<size_t>(m_nrOfItemsToDisplay, std::max<size_t>(m_listBox->getItemCount(), 1))))
                + borders.top + borders.bottom + padding.top + padding.bottom});
        else
            m_listBox->setSize({getSize().x, (m_listBox->getItemHeight() * std::max<size_t>(m_listBox->getItemCount(), 1))
                + borders.top + borders.bottom + padding.top + padding.bottom});
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::showListBox()
    {
        if (!m_listBox->isVisible() && getParent())
        {
            m_listBox->show();

            // Find the GuiContainer that contains the combo box
            Container* container = getParent();
            while (container->getParent() != nullptr)
                container = container->getParent();

            m_listBox->setPosition({getAbsolutePosition().x, getAbsolutePosition().y + getSize().y - m_bordersCached.bottom});
            container->add(m_listBox, "#TGUI_INTERNAL$ComboBoxListBox#");
            m_listBox->focus();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::hideListBox()
    {
        // If the list was open then close it now
        if (m_listBox->isVisible())
        {
            m_listBox->hide();
            m_listBox->mouseNoLongerOnWidget();

            // Find the GuiContainer in order to remove the ListBox from it
            Widget* container = this;
            while (container->getParent() != nullptr)
                container = container->getParent();

            if (container != this)
                static_cast<Container*>(container)->remove(m_listBox);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::initListBox()
    {
        m_listBox->hide();
        m_listBox->connect("ItemSelected", &ComboBox::newItemSelectedCallbackFunction, this);
        m_listBox->connect("Unfocused", &ComboBox::listBoxUnfocusedCallbackFunction, this);
        m_listBox->connect("MouseReleased", &ComboBox::hideListBox, this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::newItemSelectedCallbackFunction()
    {
        m_text.setString(m_listBox->getSelectedItem());

        m_callback.text = m_listBox->getSelectedItem();
        m_callback.itemId = m_listBox->getSelectedItemId();
        sendSignal("ItemSelected", m_listBox->getSelectedItem(), m_listBox->getSelectedItem(), m_listBox->getSelectedItemId());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::listBoxUnfocusedCallbackFunction()
    {
        if (!m_mouseHover)
            hideListBox();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        // Draw the borders
        if (m_bordersCached != Borders{0})
        {
            drawBorders(target, states, m_bordersCached, getSize(), m_borderColorCached);
            states.transform.translate({m_bordersCached.left, m_bordersCached.top});
        }

        sf::RenderStates statesForText = states;

        // Draw the background
        if (m_spriteBackground.isSet())
            m_spriteBackground.draw(target, states);
        else
            drawRectangleShape(target, states, getInnerSize(), m_backgroundColorCached);

        // Check if we have textures for the arrow
        float arrowSize;
        if (m_spriteArrowUp.isSet() && m_spriteArrowDown.isSet())
        {
            // Set the arrow like it should (down when list box is invisible, up when it is visible)
            if (m_listBox->isVisible())
            {
                arrowSize = m_spriteArrowUp.getSize().x;
                states.transform.translate({getInnerSize().x - m_paddingCached.right - arrowSize, m_paddingCached.top});

                if (m_mouseHover && m_spriteArrowUpHover.isSet())
                    m_spriteArrowUpHover.draw(target, states);
                else
                    m_spriteArrowUp.draw(target, states);
            }
            else
            {
                arrowSize = m_spriteArrowDown.getSize().x;
                states.transform.translate({getInnerSize().x - m_paddingCached.right - arrowSize, m_paddingCached.top});

                if (m_mouseHover && m_spriteArrowDownHover.isSet())
                    m_spriteArrowDownHover.draw(target, states);
                else
                    m_spriteArrowDown.draw(target, states);
            }
        }
        else // There are no textures for the arrow
        {
            arrowSize = getInnerSize().y - m_paddingCached.top - m_paddingCached.bottom;
            states.transform.translate({getInnerSize().x - m_paddingCached.right - arrowSize, m_paddingCached.top});

            if (m_mouseHover && m_arrowBackgroundColorHoverCached.isSet())
                drawRectangleShape(target, states, {arrowSize, arrowSize}, m_arrowBackgroundColorHoverCached);
            else
                drawRectangleShape(target, states, {arrowSize, arrowSize}, m_arrowBackgroundColorCached);

            sf::ConvexShape arrow{3};
            if (m_listBox->isVisible())
            {
                arrow.setPoint(0, {arrowSize / 5, arrowSize * 4 / 5});
                arrow.setPoint(1, {arrowSize / 2, arrowSize / 5});
                arrow.setPoint(2, {arrowSize * 4 / 5, arrowSize * 4 / 5});
            }
            else // The listbox is not visible, the arrow points down
            {
                arrow.setPoint(0, {arrowSize / 5, arrowSize / 5});
                arrow.setPoint(1, {arrowSize / 2, arrowSize * 4 / 5});
                arrow.setPoint(2, {arrowSize * 4 / 5, arrowSize / 5});
            }

            if (m_mouseHover && m_arrowColorHoverCached.isSet())
                arrow.setFillColor(m_arrowColorHoverCached);
            else
                arrow.setFillColor(m_arrowColorCached);

            target.draw(arrow, states);
        }

        // Draw the selected item
        if (!m_text.getString().isEmpty())
        {
            Clipping clipping{target, statesForText, {m_paddingCached.left, m_paddingCached.top}, {getInnerSize().x - m_paddingCached.left - m_paddingCached.right - arrowSize, getInnerSize().y - m_paddingCached.top - m_paddingCached.bottom}};

            statesForText.transform.translate(m_paddingCached.left, m_paddingCached.top + (((getInnerSize().y - m_paddingCached.top - m_paddingCached.bottom) - m_text.getSize().y) / 2.0f));
            m_text.draw(target, statesForText);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
