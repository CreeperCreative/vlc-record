/*****************************************************************************
 * VLCMediaList.h: VLCKit.framework VLCMediaList header
 *****************************************************************************
 * Copyright (C) 2007 Pierre d'Herbemont
 * Copyright (C) 2007 the VideoLAN team
 * $Id$
 *
 * Authors: Pierre d'Herbemont <pdherbemont # videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#import "VLCMedia.h"

/* Notification Messages */
extern NSString * VLCMediaListItemAdded;
extern NSString * VLCMediaListItemDeleted;

@class VLCMedia;
@class VLCMediaList;
@class VLCMediaListAspect;

/**
 * TODO: Documentation VLCMediaListDelegate
 */
@protocol VLCMediaListDelegate
/**
 * TODO: Documentation - [VLCMediaListDelegate mediaList:mediaAdded:atIndex:]
 */
- (void)mediaList:(VLCMediaList *)aMediaList mediaAdded:(VLCMedia *)media atIndex:(int)index;

/**
 * TODO: Documentation - [VLCMediaListDelegate mediaList:mediaRemovedAtIndex:]
 */
- (void)mediaList:(VLCMediaList *)aMediaList mediaRemovedAtIndex:(int)index;
@end

/**
 * TODO: Documentation VLCMediaList
 */
@interface VLCMediaList : NSObject
{
    void * p_mlist;                                 //< Internal instance of media list
    id <VLCMediaListDelegate,NSObject> delegate;    //< Delegate object
    /* We need that private copy because of Cocoa Bindings, that need to be working on first thread */
    NSMutableArray * cachedMedia;                   //< Private copy of media objects.
    VLCMediaListAspect * flatAspect;                //< TODO: Documentation VLCMediaList.flatAspect
    VLCMediaListAspect * hierarchicalAspect;        //< TODO: Documentation VLCMediaList.hierarchicalAspect
    VLCMediaListAspect * hierarchicalNodeAspect;    //< TODO: Documentation VLCMediaList.hierarchicalNodeAspect
}

/* Operations */
/**
 * TODO: Documentation - [VLCMediaList lock]
 */
- (void)lock;

/**
 * TODO: Documentation - [VLCMediaList unlock]
 */
- (void)unlock;

/**
 * TODO: Documentation - [VLCMediaList addMedia:]
 */
- (int)addMedia:(VLCMedia *)media;

/**
 * TODO: Documentation - [VLCMediaList insertMedia:atIndex:]
 */
- (void)insertMedia:(VLCMedia *)media atIndex:(int)index;

/**
 * TODO: Documentation - [VLCMediaList removeMediaAtIndex:]
 */
- (void)removeMediaAtIndex:(int)index;

/**
 * TODO: Documentation - [VLCMediaList mediaAtIndex:]
 */
- (VLCMedia *)mediaAtIndex:(int)index;

/**
 * TODO: Documentation - [VLCMediaList indexOfMedia:]
 */
- (int)indexOfMedia:(VLCMedia *)media;

/* Properties */
/**
 * TODO: Documentation VLCMediaList.count
 */
@property (readonly) int count;

/**
 * TODO: Documentation VLCMediaList.delegate
 */
@property (assign) id delegate;

/**
 * TODO: Documentation VLCMediaList.isReadOnly
 */
@property (readonly) BOOL isReadOnly;

/* Media list aspect */
/**
 * TODO: Documentation VLCMediaList.hierarchicalAspect
 */
@property (readonly) VLCMediaListAspect * hierarchicalAspect;

/**
 * TODO: Documentation VLCMediaList.hierarchicalNodeAspect
 */
@property (readonly) VLCMediaListAspect * hierarchicalNodeAspect;

/**
 * TODO: Documentation VLCMediaList.flatAspect
 */
@property (readonly) VLCMediaListAspect * flatAspect;
@end
