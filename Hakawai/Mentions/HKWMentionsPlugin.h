//
//  HKWMentionsPlugin.h
//  Hakawai
//
//  Copyright (c) 2014 LinkedIn Corp. All rights reserved.
//  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
//  the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
//  an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#import <Foundation/Foundation.h>

#import "HKWControlFlowPluginProtocols.h"
#import "HKWMentionsEntityProtocol.h"
#import "HKWChooserViewProtocol.h"
#import "HKWMentionsDefaultChooserViewDelegate.h"

/*!
 An attribute for \c NSAttributedString objects representing a mention. This attribute by itself confers no special
 formatting on its text; the plug-in is responsible for coloring and highlighting text according to the current state.
 */
OBJC_EXTERN NSString* _Nonnull const HKWMentionAttributeName;

/*!
 An enum representing supported modes for positioning the chooser view.

 \c HKWMentionsChooserPositionModeEnclosedTop places the chooser view inline within the text view, and locks the
 single-line viewport to the top of the text view.

 \c HKWMentionsChooserPositionModeEnclosedBottom places the chooser view inline within the text view, and locks the
 single-line viewport to the bottom of the text view.

 All the 'Custom' modes require the delegate to provide a frame for the chooser view.

 \c HKWMentionsChooserPositionModeCustomLockTopArrowPointingUp is a custom mode where the arrow is configured to point
 up and the single-line viewport is locked to the top.

 \c HKWMentionsChooserPositionModeCustomLockTopArrowPointingDown is a custom mode where the arrow is configured to point
 down and the single-line viewport is locked to the top.

 \c HKWMentionsChooserPositionModeCustomLockTopNoArrow is a custom mode where the chooser is configured with no chrome
 and the single-line viewport is locked to the top.

 \c HKWMentionsChooserPositionModeCustomLockBottomArrowPointingUp is a custom mode where the arrow is configured to
 point up and the single-line viewport is locked to the bottom.

 \c HKWMentionsChooserPositionModeCustomLockBottomArrowPointingDown is a custom mode where the arrow is configured to
 point down and the single-line viewport is locked to the bottom.

 \c HKWMentionsChooserPositionModeCustomLockBottomNoArrow is a custom mode where the chooser is configured with no
 chrome and the single-line viewport is locked to the bottom.

 \c HKWMentionsChooserPositionModeCustomNoLockArrowPointingUp is a custom mode where the arrow is configured to point
 up and the text view is never locked.

 \c HKWMentionsChooserPositionModeCustomNoLockArrowPointingDown is a custom mode where the arrow is configured to
 point down and the text view is never locked.

 \c HKWMentionsChooserPositionModeCustomNoLockNoArrow is a custom mode where the chooser is configured with no chrome
 and the text view is never locked.
 */
typedef NS_ENUM(NSInteger, HKWMentionsChooserPositionMode) {
    HKWMentionsChooserPositionModeEnclosedTop = 0,
    HKWMentionsChooserPositionModeEnclosedBottom,
    HKWMentionsChooserPositionModeCustomLockTopArrowPointingUp,
    HKWMentionsChooserPositionModeCustomLockTopArrowPointingDown,
    HKWMentionsChooserPositionModeCustomLockTopNoArrow,
    HKWMentionsChooserPositionModeCustomLockBottomArrowPointingUp,
    HKWMentionsChooserPositionModeCustomLockBottomArrowPointingDown,
    HKWMentionsChooserPositionModeCustomLockBottomNoArrow,
    HKWMentionsChooserPositionModeCustomNoLockArrowPointingUp,
    HKWMentionsChooserPositionModeCustomNoLockArrowPointingDown,
    HKWMentionsChooserPositionModeCustomNoLockNoArrow
};

/*!
 An enum representing possible states that the mentions plug-in may be in.
 */
typedef NS_ENUM(NSInteger, HKWMentionsPluginState) {
    HKWMentionsPluginStateQuiescent,
    HKWMentionsPluginStateCreatingMention
};

@class HKWMentionsPlugin;
/*!
 A protocol providing a way for listeners to be informed when the state of the mentions plug-in changes. This allows
 a host application to know when the plug-in is creating a mention, and when it is quiescent.
 */
@protocol HKWMentionsStateChangeDelegate <NSObject>

@optional

/// Inform the delegate that the specified mentions plug-in changed its internal state.
- (void)mentionsPlugin:(HKWMentionsPlugin *_Null_unspecified)plugin
        stateChangedTo:(HKWMentionsPluginState)newState
                  from:(HKWMentionsPluginState)oldState;

/// Inform the delegate that the specified mentions plug-in is about to activate and display its chooser view.
- (void)mentionsPluginWillActivateChooserView:(HKWMentionsPlugin *_Null_unspecified)plugin;

/// Inform the delegate that the specified mentions plug-in activated and displayed its chooser view.
- (void)mentionsPluginActivatedChooserView:(HKWMentionsPlugin *_Null_unspecified)plugin;

/// Inform the delegate that the specified mentions plug-in deactivated and hid its chooser view.
- (void)mentionsPluginDeactivatedChooserView:(HKWMentionsPlugin *_Null_unspecified)plugin;

/*! 
 Inform the delegate that the specified mentions plug-in created a mention at the given location as a result of user
 input.

 \note Mentions created by calling the \c addMention: or \c addMentions: methods will not trigger this method.
 */
- (void)mentionsPlugin:(HKWMentionsPlugin *_Null_unspecified)plugin
        createdMention:(id<HKWMentionsEntityProtocol> _Null_unspecified)entity
            atLocation:(NSUInteger)location;

/*!
 Inform the delegate that the specified mentions plug-in trimmed a mention at the given location as a result of user
 input.
 */
- (void)mentionsPlugin:(HKWMentionsPlugin *_Null_unspecified)plugin
        trimmedMention:(id<HKWMentionsEntityProtocol> _Null_unspecified)entity
            atLocation:(NSUInteger)location;

/*!
 Inform the delegate that the specified mentions plug-in deleted a mention at the given location as a result of user
 input.
 */
- (void)mentionsPlugin:(HKWMentionsPlugin *_Null_unspecified)plugin
        deletedMention:(id<HKWMentionsEntityProtocol> _Null_unspecified)entity
            atLocation:(NSUInteger)location;

/*!
 Inform the delegate an entity was selected as a result of user input.
 */
- (void)selected:(id<HKWMentionsEntityProtocol> _Null_unspecified)entity
     atIndexPath:(NSIndexPath *_Null_unspecified)indexPath;

@end

@class HKWMentionsAttribute;

@interface HKWMentionsPlugin : NSObject <HKWDirectControlFlowPluginProtocol>

@property (nonatomic, weak, nullable) id<HKWMentionsDefaultChooserViewDelegate> delegate;
@property (nonatomic, weak, nullable) id<HKWMentionsStateChangeDelegate> stateChangeDelegate;

/*!
 Instantiate a mentions plug-in with the specified chooser mode, no control characters, and a default search length of
 3 characters.
 */
+ (nonnull instancetype)mentionsPluginWithChooserMode:(HKWMentionsChooserPositionMode)mode;

/*!
 Instantiate a mentions plug-in with the specified chooser mode, control character set, and search length.

 \param controlCharacterSet    a \c NSCharacterSet containing the character or characters that should be used to begin
                               an explicit mention, or nil if explicit mentions should not be enabled
 \param searchLength           the number of characters to wait before beginning an implicit mention, or 0 or a negative
                               value if implicit mentions should not be enabled
 */
+ (nonnull instancetype)mentionsPluginWithChooserMode:(HKWMentionsChooserPositionMode)mode
                            controlCharacters:(NSCharacterSet *_Null_unspecified)controlCharacterSet
                                 searchLength:(NSInteger)searchLength;

/*!
 Instantiate a mentions plug-in with the specified chooser mode, control character set, search length, a color for
 unselected mentions text, and a background color and text color for selected mentions text.
 */
+ (nonnull instancetype)mentionsPluginWithChooserMode:(HKWMentionsChooserPositionMode)mode
                            controlCharacters:(NSCharacterSet *_Null_unspecified)controlCharacterSet
                                 searchLength:(NSInteger)searchLength
                              unselectedColor:(UIColor *_Null_unspecified)unselectedColor
                                selectedColor:(UIColor *_Null_unspecified)selectedColor
                      selectedBackgroundColor:(UIColor *_Null_unspecified)selectedBackgroundColor;

/*!
 Instantiate a mentions plug-in with the specified chooser mode, control character set, search length, custom attributes
 to apply to unselected mentions, and custom attributes to apply to selected mentions.
 */
+ (nonnull instancetype)mentionsPluginWithChooserMode:(HKWMentionsChooserPositionMode)mode
                            controlCharacters:(NSCharacterSet *_Null_unspecified)controlCharacterSet
                                 searchLength:(NSInteger)searchLength
                  unselectedMentionAttributes:(NSDictionary *_Null_unspecified)unselectedAttributes
                    selectedMentionAttributes:(NSDictionary *_Null_unspecified)selectedAttributes;

#pragma mark - API

/*!
 Inform the plugin that the textview was programatically updated (e.g. setText: or setAttributedText:)
 */
-(void)textViewDidProgrammaticallyUpdate:(UITextView *_Null_unspecified)textView;

/*!
 Extract mentions attributes from an attributed string. The array of mentions attribute objects returned by this method
 can be passed directly into the \c addMentions: method on the plug-in.
 */
+ (NSArray *_Null_unspecified)mentionsAttributesInAttributedString:(NSAttributedString *_Null_unspecified)attributedString;

/*!
 Return an array of \c HKWMentionsAttribute objects corresponding to the mentions attributes which currently exist in
 the plug-in's parent text view.

 \warning This method will return an empty array if the plug-in isn't registered to a text view.
 */
- (NSArray *_Null_unspecified)mentions;

/*!
 Add a mention attribute to the parent text view's text. This method is intended to be called when the text view is
 first being populated with text (for example, when a user decides to edit an existing document containing mentions).

 \param mention    a mention object representing the mention to add. The \c range property on the object must be set
                   properly. In addition, the length of \c range must match the length of the \c mentionText property,
                   and the existing text in the text view at \c range must exactly match the value of the \c mentionText
                   string. Invalid mentions are ignored.
 */
- (void)addMention:(HKWMentionsAttribute *_Null_unspecified)mention;

/*!
 Add multiple mentions to the parent text view's text. This is a convenience method that calls the \c addMention: method
 for each element in \c mentions that passes typechecking.
 */
- (void)addMentions:(NSArray *_Null_unspecified)mentions;


#pragma mark - Behavior Configuration

/*!
 Whether or not the text view delegate's \c textViewDidChange: method should be called whenever a mention is added as a
 result of user action.

 \note Mentions created by calling the \c addMention: or \c addMentions: methods will not trigger the method.
 */
@property (nonatomic) BOOL notifyTextViewDelegateOnMentionCreation;

/*!
 Whether or not the text view delegate's \c textViewDidChange: method should be called whenever a mention is trimmed.
 */
@property (nonatomic) BOOL notifyTextViewDelegateOnMentionTrim;

/*!
 Whether or not the text view delegate's \c textViewDidChange: method should be called whenever a mention is deleted.
 */
@property (nonatomic) BOOL notifyTextViewDelegateOnMentionDeletion;

/*!
 Whether or not to allow resumption of mentions creation upon resuming editing.
 */
@property (nonatomic) BOOL resumeMentionsCreationEnabled;

/*!
 Whether or not we should continue searching for an explicit mention after we get back empty results. If this
 is off, empty results will return the mentions creation state to \c HKWMentionsPluginStateQuiescent. If this is
 on, empty results won't modify the mentions creation state.
 */
@property (nonatomic) BOOL shouldContinueSearchingAfterEmptyResults;

#pragma mark - API for HKWMentionsCustomChooserViewDelegate

/*!
 Informs the plugin typeahead results are returned, so it can update its internal state accordingly.
 It's meant to be an experimental feature. This function will be updated.
*/
- (void)dataReturnedWithEmptyResults:(BOOL)isEmptyResults
         keystringEndsWithWhiteSpace:(BOOL)keystringEndsWithWhiteSpace;

/*!
Handles the selection from the user. This is only needed for consumers who use custom chooser view.
*/
- (void)handleSelectionForEntity:(nonnull id<HKWMentionsEntityProtocol>)entity;

#pragma mark - Chooser UI Configuration

/*! 
 The class of the chooser view to instantiate. Must be a subclass of the \c UIView class. If set to an invalid value or
 nil, defaults to the built-in chooser view.
 
 \note Set this when the mentions plug-in is created; setting it will do nothing after the chooser view is instantiated
 the first time.

 \note Per the definition of \c HKWChooserViewProtocol your custom chooser view class (if any) can either choose to
 consume the plug-in's \c UITableViewDelegate and \c UITableViewDataSource methods.
 Use the former if you want to use the API in \c HKWMentionsDefaultChooserViewDelegate to provide table view cells and cell heights, the
 latter if you want your chooser view to be completely responsible for preparing the UI.
 */
@property (nonatomic) Class<HKWChooserViewProtocol> _Null_unspecified chooserViewClass;

/*!
 Return the frame of the chooser view. If the chooser view hasn't been instantiated, the null rect will be returned.
 */
- (CGRect)chooserViewFrame;

/*!
 Return a generic reference to the chooser view.
 */
@property (nonatomic, readonly) UIView<HKWChooserViewProtocol> *_Null_unspecified chooserView;

/*!
 Return the chooser position mode the chooser was configured with.
 */
@property (nonatomic, readonly) HKWMentionsChooserPositionMode chooserPositionMode;

/*!
 The background color of the chooser view. If the chooser view hasn't been instantiated, the color will be applied to
 the chooser view upon instantiation.
 */
@property (nonatomic, strong, nonnull) UIColor *chooserViewBackgroundColor;

@property (nonatomic) UIEdgeInsets chooserViewEdgeInsets;

/*!
 Whether or not the delegate supports displaying a loading cell in the chooser view while waiting for the results
 */
@property (nonatomic, readonly) BOOL loadingCellSupported;

/*!
 If the plug-in is set to display the chooser view in a custom position, set the top level view and a block to be called
 after the view is attached to its superview (intended to be used to set up layout constraints). The argument to the
 block is a generic reference to the chooser view.

 \warning Attach the plug-in to a text view before calling this method. Calling this method on a plug-in instance that
 hasn't been registered to a text view is a no-op.
 */
- (void)setChooserTopLevelView:(UIView *_Null_unspecified)topLevelView attachmentBlock:(void(^_Null_unspecified)(UIView *_Null_unspecified))block;

/*!
 Return a rect describing the frame that would be assigned to the chooser view if in one of the preset modes, or
 \c CGRectNull otherwise.

 \warning Attach the plug-in to a text view before calling this method. Calling this method on a plug-in instance that
 hasn't been registered to a text view return the null rect.
 */
- (CGRect)calculatedChooserFrameForMode:(HKWMentionsChooserPositionMode)mode
                             edgeInsets:(UIEdgeInsets)edgeInsets;

/*!
 Gets the input made by the user for both @ mentions or # hashtag.
 Needs to be public for integration between Hakawai and HotPot.
 */
- (unichar)getExplicitSearchControlCharacter;

@end
