//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Apr 19 2021
//
//
/*
..... This file started from: https://github.com/JamesBoer/ImFrame .....
*/
#include <vector>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "ImUtilities.h"


namespace ImUtilities {
    static NSInteger s_selected_tag_id = -1;
}

@interface MenuItemHandler : NSObject
-(void) OnClick: (id) sender;
@end

@implementation MenuItemHandler
-(void) OnClick: (id) sender {
    NSMenuItem* menu_item = sender;
    ImUtilities::s_selected_tag_id = menu_item.tag;
}
@end


#include <iostream>


namespace ImUtilities {
    
    // Built Menu Variables
    static NSMenu*                  s_menu_bar;
    
    // Click Handler
    static MenuItemHandler*         s_menu_handler;
    static NSInteger                s_current_tag_id = 1;

    // Local Flags
    static bool         s_build_menus = true;
    static bool         s_clear_menus = false;


    // Init Mac Main Menu Bar, add "Quit" item
    void osxMenuInitialize(const char* app_name) {
        // !!!!! Code from https://github.com/floooh/sokol/pull/362:
        NSMenu* menu_bar = [[NSMenu alloc] init];                           // Main Menu Bar
        NSMenuItem* app_menu_item = [[NSMenuItem alloc] init];              // "Drop"
            NSMenu* app_menu = [[NSMenu alloc] init];                       // Sub Menu
            NSString* window_title_as_nsstring = [NSString stringWithUTF8String:app_name];
            NSString* quit_title = [@"Quit " stringByAppendingString:window_title_as_nsstring];
            NSMenuItem* quit_menu_item = [[NSMenuItem alloc]                // "Quit Drop"
                initWithTitle:quit_title
                action:@selector(terminate:)
                keyEquivalent:@"q"];
        [app_menu addItem:quit_menu_item];
        app_menu_item.submenu = app_menu;
        [menu_bar addItem:app_menu_item];
        NSApp.mainMenu = menu_bar;
        // !!!!! End Code
       
        // Get ready to build menu
        s_menu_handler = [[MenuItemHandler alloc] init];//[[MenuItemHandler new] autorelease];
        s_menu_bar = menu_bar;
    }

    // Called when App is closing to clean up menu
    void osxMenuShutDown() {
        s_menu_handler = nullptr;
    }


    // Call before adding menu's to Main Menu Bar
    bool osxBeginMainMenuBar() {
        return true;
    }

    void osxEndMainMenuBar() {
        // Clear menus if flag was set, preserve the first menu ("AppName")
        if (s_clear_menus) {
            while (s_menu_bar.itemArray.count > 1) {
                [s_menu_bar removeItemAtIndex:1];
            }
            s_clear_menus = false;
        }
    }


    bool osxBeginMenu(const char* label, bool enabled) {
        if (s_clear_menus) return false;

        // Preserve only characters up to ## in label
        NSString* label_str = [NSString stringWithUTF8String:label];
        NSUInteger loc = [label_str rangeOfString:@"##"].location;
        if (loc < [label_str length]) label_str = [label_str substringToIndex:loc];

        //[[NSApp mainMenu] insertItem:menuBarItem atIndex:3];

        NSUInteger find_menu = [s_menu_bar indexOfItemWithTitle:label_str];
        if (find_menu != -1) {
            if (s_build_menus) {
                s_build_menus = false;
                std::cout << "Done building menu..." << std::endl;
            }
        }


        NSUInteger idx = [s_menu_bar numberOfItems] - 1;

        // Create a new menu_item and add to menu
        if (s_build_menus) {
            // New submenus consist of both a new menu and a new menu_item to trigger it
            NSMenu* new_menu = [[NSMenu alloc] init];//[[NSMenu new] autorelease];
                new_menu.autoenablesItems = false;
                new_menu.title = label_str;
            NSMenuItem* menu_item = [[NSMenuItem alloc] init];//[[NSMenuItem new] autorelease];
                menu_item.title = label_str;
            [menu_item setSubmenu:new_menu];

            // Add item to menu bar
            [s_menu_bar insertItem:menu_item atIndex:idx];
        }
        
        NSMenuItem* menu_item = [s_menu_bar itemAtIndex:idx];

        if (menu_item) {    
            menu_item.enabled = enabled;                                    // Update enabled state
        }
        // // If the labels don't match, something has dynamically changed and we need to clear and rebuild the menus
        // if (![label_str isEqualToString:menu_item.title]) {
        //     s_clear_menus = true;
        //     return false;
        // }
                
                        
        // Unlike Dear ImGui menus, we typically return true so clicks can be checked by any menu_item
        return true;
    }

    void osxEndMenu() {
        // // Check to see if the number of menu items doesn't agree.  If so, something has
        // // been dynamically added or removed, and we need to clear the menus and rebuild.
        // NSMenu* menu = s_menus.back().menu;
        // NSUInteger idx = s_menus.back().idx;
        // if (menu.itemArray.count != idx) s_clear_menus = true;
        
        // // We've finished this menu, so pop tracking data off the menu stack
        // s_menus.pop_back();
    }

    bool osxMenuItem(const char* label, const char* shortcut, bool selected, bool enabled) {
        // if (s_clear_menus) return false;
        
        // // Save off current index value, and increment index on stack
        // NSInteger idx = s_menus.back().idx;
        // s_menus.back().idx++;
        
        // // Preserve only characters up to ## in label
        // NSString* label_str = [NSString stringWithUTF8String:label];
        // NSUInteger loc = [label_str rangeOfString:@"##"].location;
        // if (loc < [label_str length]) label_str = [label_str substringToIndex:loc];
            
        // // If we're in a building pass, create new NSMenuItem and insert into menu
        // if (s_build_menus) {
        //     NSMenuItem* menu_item = [[NSMenuItem alloc] init];//[[NSMenuItem new] autorelease];
        //         menu_item.title = label_str;
        //     [menu_item setTag:s_current_tag_id];
        //     ++s_current_tag_id;
        //     menu_item.action = @selector(OnClick:);
        //     menu_item.target = s_menu_handler;
        //     // We'll assume the shortcut isn't going to dynamically change
        //     if (shortcut) menu_item.keyEquivalent = [NSString stringWithUTF8String:shortcut];
        //     [s_menus.back().menu addItem:menu_item];
        // }
        
        // // Get the current menu item
        // NSMenuItem* menu_item = [s_menus.back().menu itemAtIndex:idx];
        
        // // If the labels don't match, something has dynamically changed and we need to clear and rebuild the menus
        // if (![label_str isEqualToString:menu_item.title]) {
        //     s_clear_menus = true;
        //     return false;
        // }
        
        // // Set enabled and selected properties
        // menu_item.enabled = enabled;
        // menu_item.state = selected ? NSControlStateValueOn : NSControlStateValueOff;
        
        // // Check for click only if we're enabled
        // if (enabled) {
        //     // We use the tag integer to uniquely identify which menu_item was clicked, since
        //     // they all use the same handler target object.
        //     if ([menu_item tag] == s_selected_tag_id) {
        //         // Reset selected tag and return true, indicating we've clicked on this menu_item
        //         s_selected_tag_id = -1;
        //         return true;
        //     }
        // }

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
        // if (s_clear_menus) return;
        
        // // If we're in a building pass, create new NSMenuItem as separator and insert into menu
        // if (s_build_menus) {
        //     NSMenuItem* menu_item = [NSMenuItem separatorItem];
        //     //[s_menu_bar.items.back().submenu addItem:menu_item];
        // }
    }
    
}
