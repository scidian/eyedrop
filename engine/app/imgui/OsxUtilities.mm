//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Apr 19 2021
//
//
/*
The MIT License (MIT)

Copyright (c) 2021 James Boer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <vector>
#include "MainMenu.h"


namespace MainMenu {
    static NSInteger s_selected_tag_id = -1;
}

@interface MenuItemHandler : NSObject
-(void) OnClick: (id) sender;
@end

@implementation MenuItemHandler
-(void) OnClick: (id) sender
{
    NSMenuItem* menu_item = sender;
    MainMenu::s_selected_tag_id = menu_item.tag;
}
@end


#include <iostream>


namespace MainMenu {
    
    struct MenuState {
        NSMenu*     menu = nullptr;
        NSUInteger  idx = 0;
    };

    static MenuItemHandler*         s_menu_handler;
    static std::vector<MenuState>   s_menus;
    static NSInteger    s_current_tag_id = 1;
    static bool         s_build_menus = true;
    static bool         s_clear_menus = false;

    void osxMenuInitialize(const char* app_name) {
        // !!!!! Code from https://github.com/floooh/sokol/pull/362:
        NSMenu* menu_bar = [[NSMenu new] autorelease];
        NSMenuItem* app_menu_item = [[NSMenuItem new] autorelease];
            NSMenu* app_menu = [[NSMenu new] autorelease];
            NSString* window_title_as_nsstring = [NSString stringWithUTF8String:app_name];
            NSString* quit_title = [@"Quit " stringByAppendingString:window_title_as_nsstring];
            NSMenuItem* quit_menu_item = [[NSMenuItem alloc]
                initWithTitle:quit_title
                action:@selector(terminate:)
                keyEquivalent:@"q"];
        [app_menu addItem:quit_menu_item];
        app_menu_item.submenu = app_menu;
        [menu_bar addItem:app_menu_item];
        NSApp.mainMenu = menu_bar;
        // !!!!! End Code
       

        s_menu_handler = [[MenuItemHandler new] autorelease];
        s_menus.push_back(MenuState());
        s_menus.back().menu = [NSApp menu];
    }

    void osxMenuShutDown() {
        s_menu_handler = nullptr;
        s_menus.clear();
    }

    bool osxBeginMainMenuBar() {
        assert(s_menus.size() == 1);
        s_menus.front().idx = 1;
        s_current_tag_id = 1;
        return true;
    }

    void osxEndMainMenuBar() {
        assert(s_menus.size() == 1);
        
        // By default, we'll want to clear the build flag at this point.
        s_build_menus = false;
        
        // Check for both the clear menu flag being set, as well as the number of menuitems in the topmost menu being changed.
        NSMenu* mainMenuBar = s_menus.front().menu;

        // Note idx + 1, because we started at one, and we never reached the last "Window" menu.       
        NSUInteger checkedCount = s_menus.back().idx + 1;
        if (s_clear_menus || mainMenuBar.itemArray.count != checkedCount) {
            // This is sort of weird, I know.  We want to preserve the first and last menus ("AppName" and Window), which we didn't create.
            // But we need to clear everything between those two.  Thus, the weird clearing code here.
            while (mainMenuBar.itemArray.count > 2)
                [mainMenuBar removeItemAtIndex:1];
            
            // All custom menus are cleared, so we're reset the clear flag and trigger a rebuild pass on the next update.
            s_clear_menus = false;
            s_build_menus = true;
        }
    }

    bool osxBeginMenu(const char* label, bool enabled) {
        if (s_clear_menus) return false;
        
        NSMenu* menu = s_menus.back().menu;
        NSInteger idx = s_menus.back().idx;
        s_menus.back().idx++;

        // Preserve only characters up to ## in label
        NSString* label_str = [NSString stringWithUTF8String:label];
        NSUInteger loc = [label_str rangeOfString:@"##"].location;
        if (loc < [label_str length]) label_str = [label_str substringToIndex:loc];

        // Create a new menu_item and add to menu
        if (s_build_menus) {
            // New submenus consist of both a new menu and a new menu_item to trigger it
            NSMenu* newMenu = [[NSMenu new] autorelease];
                newMenu.autoenablesItems = false;
                newMenu.title = label_str;
            NSMenuItem* menu_item = [[NSMenuItem new] autorelease];
                menu_item.title = label_str;
            [menu_item setSubmenu:newMenu];
            [menu insertItem:menu_item atIndex:idx];
        }
        
        NSMenuItem* menu_item = [menu itemAtIndex:idx];
        
        // If the labels don't match, something has dynamically changed and we need to clear and rebuild the menus
        if (![label_str isEqualToString:menu_item.title]) {
            s_clear_menus = true;
            return false;
        }
        
        // Update enabled state
        menu_item.enabled = enabled;
        
        // Push this menu on the stack for tracking purposes
        s_menus.push_back(MenuState());
        s_menus.back().menu = menu_item.submenu;
        
        // Unlike Dear ImGui menus, we typically return true so clicks can be checked by any menu_item
        return true;
    }

    void osxEndMenu() {
        // Check to see if the number of menu items doesn't agree.  If so, something has
        // been dynamically added or removed, and we need to clear the menus and rebuild.
        NSMenu* menu = s_menus.back().menu;
        NSUInteger idx = s_menus.back().idx;
        if (menu.itemArray.count != idx) s_clear_menus = true;
        
        // We've finished this menu, so pop tracking data off the menu stack
        s_menus.pop_back();
    }

    bool osxMenuItem(const char* label, const char* shortcut, bool selected, bool enabled) {
        if (s_clear_menus) return false;
        
        // Save off current index value, and increment index on stack
        NSInteger idx = s_menus.back().idx;
        s_menus.back().idx++;
        
        // Preserve only characters up to ## in label
        NSString* label_str = [NSString stringWithUTF8String:label];
        NSUInteger loc = [label_str rangeOfString:@"##"].location;
        if (loc < [label_str length]) label_str = [label_str substringToIndex:loc];
            
        // If we're in a building pass, create new NSMenuItem and insert into menu
        if (s_build_menus) {
            NSMenuItem* menu_item = [[NSMenuItem new] autorelease];
                menu_item.title = label_str;
            [menu_item setTag:s_current_tag_id];
            ++s_current_tag_id;
            menu_item.action = @selector(OnClick:);
            menu_item.target = s_menu_handler;
            // We'll assume the shortcut isn't going to dynamically change
            if (shortcut) menu_item.keyEquivalent = [NSString stringWithUTF8String:shortcut];
            [s_menus.back().menu addItem:menu_item];
        }
        
        // Get the current menu item
        NSMenuItem* menu_item = [s_menus.back().menu itemAtIndex:idx];
        
        // If the labels don't match, something has dynamically changed and we need to clear and rebuild the menus
        if (![label_str isEqualToString:menu_item.title]) {
            s_clear_menus = true;
            return false;
        }
        
        // Set enabled and selected properties
        menu_item.enabled = enabled;
        menu_item.state = selected ? NSControlStateValueOn : NSControlStateValueOff;
        
        // Check for click only if we're enabled
        if (enabled) {
            // We use the tag integer to uniquely identify which menu_item was clicked, since
            // they all use the same handler target object.
            if ([menu_item tag] == s_selected_tag_id) {
                // Reset selected tag and return true, indicating we've clicked on this menu_item
                s_selected_tag_id = -1;
                return true;
            }
        }
        return false;
    }

    bool osxMenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled) {
        if (osxMenuItem(label, shortcut, p_selected ? *p_selected : false, enabled)) {
            if (p_selected) *p_selected = !*p_selected;
            return true;
        }
        return false;
    }

    void osxSeparator() {
        if (s_clear_menus) return;
        
        // Increment index on stack
        s_menus.back().idx++;
        
        // If we're in a building pass, create new NSMenuItem as separator and insert into menu
        if (s_build_menus) {
            NSMenuItem* menu_item = [NSMenuItem separatorItem];
            [s_menus.back().menu addItem:menu_item];
        }
    }
    
}
