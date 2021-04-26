//####################################################################################
//
// Description:     ImMenu, ImGui to MacOS Main Menu Bar Wrapper
// Author:          Stephens Nunnally and Scidian Software
// License:         Distributed under the MIT License
// Source(s):       https://github.com/stevinz/immenu
// Original Idea:   https://github.com/JamesBoer/ImFrame
//
// Copyright (c) 2021 Stephens Nunnally and Scidian Software
// Copyright (c) 2021 James Boer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//####################################################################################
// Includes
#include <vector>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "engine/app/image/Image.h"
#include "ImMenu.h"

// Free Function
#if __has_feature(objc_arc)
    #define _IMMENU_OBJC_RELEASE(obj) { obj = nil; }
#else
    #define _IMMENU_OBJC_RELEASE(obj) { [obj release]; obj = nil; }
#endif


//####################################################################################
//##    Click Handler
//####################################################################################
namespace ImMenu {
    static NSInteger s_selected_tag_id = -1;
}

@interface MenuItemHandler : NSObject
-(void) OnClick: (id) sender;
@end

@implementation MenuItemHandler
-(void) OnClick: (id) sender {
    NSMenuItem* menu_item = sender;
    ImMenu::s_selected_tag_id = menu_item.tag;
}
@end


//####################################################################################
//##    ImMenu
//##        ImGui wrapper for OS specific functions
//####################################################################################
namespace ImMenu {
    

//####################################################################################
//##    Static Variables
//####################################################################################
// Menu Variables
static NSMenu*                  s_menu_bar;
static NSInteger                s_menu_index = 0;
static NSInteger                s_item_index = 0;

// Click Handler
static MenuItemHandler*         s_menu_handler;
static NSInteger                s_current_tag_id = 1;

// Flags
static bool                     s_build_menu = false;

//####################################################################################
//##    Init
//####################################################################################
// Init Mac Main Menu Bar, add "Quit" item
void osxMenuInitialize(const char* app_name) {
    // Text to use for "Quit" menu item
    NSString* quit_title = [@"Quit " stringByAppendingString:[NSString stringWithUTF8String:app_name]];

    // Create new MacOS main menu bar
    NSMenu* menu_bar = [[NSMenu alloc] init];
        // First item to be added to menu bar, "App Name"
        NSMenuItem* app_menu_item = [[NSMenuItem alloc] init];
            // Sub Menu to be attached to "App Name" menu item
            NSMenu* app_menu = [[NSMenu alloc] init];
                // "Quit" menu item to be added to "App Name" sub menu
                NSMenuItem* quit_menu_item = [[NSMenuItem alloc]
                    initWithTitle:quit_title
                    action:@selector(terminate:)
                    keyEquivalent:@"q"];
            [app_menu addItem:quit_menu_item];
        app_menu_item.submenu = app_menu;
    [menu_bar addItem:app_menu_item];
    
    // Init Menu
    NSApp.mainMenu = menu_bar;                                                      // Apply menu bar to our Application
    s_menu_bar = menu_bar;                                                          // Save reference to menu bar
    s_menu_handler = [[MenuItemHandler alloc] init];                                // Initialize click handler
}

// Called when App is closing to clean up menu
void osxMenuShutDown() {
    _IMMENU_OBJC_RELEASE(s_menu_handler);
    _IMMENU_OBJC_RELEASE(s_menu_bar);
}


//####################################################################################
//##    Begin / End Menu Bar
//####################################################################################
// Call before adding menu's to Main Menu Bar
bool osxBeginMainMenuBar() {
    return true;
}

void osxEndMainMenuBar() {
    // Clear menus if flag was set, preserve the first menu ("AppName")
    // if (s_clear_menus) {
    //     while (s_menu_bar.itemArray.count > 1) {
    //         [s_menu_bar removeItemAtIndex:1];
    //     }
    //     s_clear_menus = false;
    // }
}

//####################################################################################
//##    Begin / End Menu
//####################################################################################
bool osxBeginMenu(const char* label, bool enabled) {
    // Label as NSString
    NSString* title = [NSString stringWithUTF8String:label];

    // Check if menu exists
    s_menu_index = [s_menu_bar indexOfItemWithTitle:title];
    
    // Does exist, do not build
    if (s_menu_index != -1) {
        s_build_menu = false;

    // Does not exist, build now
    } else {
        s_build_menu = true;

        // New submenus consist of both a new menu and a new menu_item to trigger it
        NSMenu* new_menu = [[NSMenu alloc] init];
            new_menu.autoenablesItems = false;
            new_menu.title = title;
        NSMenuItem* menu_item = [[NSMenuItem alloc] init];
            menu_item.title = title;
        [menu_item setSubmenu:new_menu];

        // Add item to menu bar
        s_menu_index = [s_menu_bar numberOfItems];
        [s_menu_bar insertItem:menu_item atIndex:s_menu_index];
    }

    // Update 'enabled' property
    NSMenuItem* menu_bar_item = [s_menu_bar itemAtIndex:s_menu_index];
    if (menu_bar_item) {    
        menu_bar_item.enabled = enabled;
    }
                                
    // Reset item index
    s_item_index = 0;

    // Return true so clicks can be handled by any clicked NSMenuItem
    return true;
}

void osxEndMenu() {
    s_build_menu = false;
}


//####################################################################################
//##    Menu Items
//####################################################################################
bool osxMenuItem(const char* label, const char* shortcut, bool selected, bool enabled, DrImage* image) {
    // Label as NSString
    NSString* title = [NSString stringWithUTF8String:label];

    // Get current Main Menu Item
    NSMenuItem* menu_bar_item = [s_menu_bar itemAtIndex:s_menu_index];

    // Build if in building pass
    if (s_build_menu) {
        // Create new menu item
        NSMenuItem* menu_item = [[NSMenuItem alloc] init];
            menu_item.title = title;
            menu_item.action = @selector(OnClick:);
            menu_item.target = s_menu_handler;
        [menu_item setTag:s_current_tag_id];
        ++s_current_tag_id;
        if (shortcut) menu_item.keyEquivalent = [NSString stringWithUTF8String:shortcut];

        // Set menu item icon
        if (image != nullptr) {
            ImageDescriptor icon_desc { };
                icon_desc.width =   image->bitmap().width;
                icon_desc.height =  image->bitmap().height;
                icon_desc.ptr =    &image->bitmap().data[0];
                icon_desc.size =    image->bitmap().data.size();
            NSImage* nsimage = (NSImage*)osxCreateImage(&icon_desc, 24, 24);
            [ menu_item setImage:nsimage ];
        }

        // Add Item to Menu
        [menu_bar_item.submenu addItem:menu_item];
    }
    
    // Set 'enable' and 'selected' properties
    NSMenuItem* menu_item = [menu_bar_item.submenu itemAtIndex:s_item_index];
    if (menu_item) {
        menu_item.enabled = enabled;
        menu_item.state = selected ? NSControlStateValueOn : NSControlStateValueOff;
    }
    
    // If enabled, see if it was clicked
    if (enabled) {
        // Compare handler tag to current item tag to see if has been clicked
        if ([menu_item tag] == s_selected_tag_id) {
            // Reset handler tag and return true to signify menu item has been clicked
            s_selected_tag_id = -1;
            return true;
        }
    }

    s_item_index++;
    return false;
}

bool osxMenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled, DrImage* image) {
    if (osxMenuItem(label, shortcut, p_selected ? *p_selected : false, enabled, image)) {
        if (p_selected) *p_selected = !(*p_selected);
        return true;
    }
    return false;
}

void osxSeparator() {
    if (s_build_menu) {
        NSMenuItem* seperator_item = [NSMenuItem separatorItem];
        NSMenuItem* menu_bar_item = [s_menu_bar itemAtIndex:s_menu_index];
        [menu_bar_item.submenu addItem:seperator_item];
    }
    s_item_index++;
}


//####################################################################################
//##    Builder Functions
//####################################################################################
void* osxCreateImage(const ImageDescriptor* image_desc, const int width, const int height) {
    CGColorSpaceRef cg_color_space = CGColorSpaceCreateDeviceRGB();
    CFDataRef cf_data = CFDataCreate(kCFAllocatorDefault, (const UInt8*)image_desc->ptr, (CFIndex)image_desc->size);
    CGDataProviderRef cg_data_provider = CGDataProviderCreateWithCFData(cf_data);
    CGImageRef cg_img = CGImageCreate(
        (size_t)image_desc->width,                          // width
        (size_t)image_desc->height,                         // height
        8,                                                  // bitsPerComponent
        32,                                                 // bitsPerPixel
        (size_t)image_desc->width * 4,                      // bytesPerRow
        cg_color_space,                                     // space
        kCGImageAlphaLast | kCGImageByteOrderDefault,       // bitmapInfo
        cg_data_provider,                                   // provider
        NULL,                                               // decode
        false,                                              // shouldInterpolate
        kCGRenderingIntentDefault);
    CFRelease(cf_data);
    CGDataProviderRelease(cg_data_provider);
    CGColorSpaceRelease(cg_color_space);

    NSSize desired_size;
        desired_size.width =  width;
        desired_size.height = height;
    NSImage* ns_image = [[NSImage alloc] initWithCGImage:cg_img size:desired_size];
    CGImageRelease(cg_img);
    return ns_image;
}


}   // End ImMenu
