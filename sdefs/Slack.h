/*
 * Slack.h
 */

#import <AppKit/AppKit.h>
#import <ScriptingBridge/ScriptingBridge.h>


@class SlackItem, SlackApplication, SlackColor, SlackDocument, SlackWindow, SlackAttributeRun, SlackCharacter, SlackParagraph, SlackText, SlackAttachment, SlackWord, SlackPrintSettings;

enum SlackSavo {
	SlackSavoAsk = 'ask ' /* Ask the user whether or not to save the file. */,
	SlackSavoNo = 'no  ' /* Do not save the file. */,
	SlackSavoYes = 'yes ' /* Save the file. */
};
typedef enum SlackSavo SlackSavo;

enum SlackEnum {
	SlackEnumStandard = 'lwst' /* Standard PostScript error handling */,
	SlackEnumDetailed = 'lwdt' /* print a detailed report of PostScript errors */
};
typedef enum SlackEnum SlackEnum;

@protocol SlackGenericMethods

- (void) closeSaving:(SlackSavo)saving savingIn:(NSURL *)savingIn;  // Close an object.
- (void) delete;  // Delete an object.
- (void) duplicateTo:(SBObject *)to withProperties:(NSDictionary *)withProperties;  // Copy object(s) and put the copies at a new location.
- (BOOL) exists;  // Verify if an object exists.
- (void) moveTo:(SBObject *)to;  // Move object(s) to a new location.
- (void) saveAs:(NSString *)as in:(NSURL *)in_;  // Save an object.

@end



/*
 * Standard Suite
 */

// A scriptable object.
@interface SlackItem : SBObject <SlackGenericMethods>

@property (copy) NSDictionary *properties;  // All of the object's properties.


@end

// An application's top level scripting object.
@interface SlackApplication : SBApplication

- (SBElementArray<SlackDocument *> *) documents;
- (SBElementArray<SlackWindow *> *) windows;

@property (readonly) BOOL frontmost;  // Is this the frontmost (active) application?
@property (copy, readonly) NSString *name;  // The name of the application.
@property (copy, readonly) NSString *version;  // The version of the application.

- (SlackDocument *) open:(NSURL *)x;  // Open an object.
- (void) print:(NSURL *)x printDialog:(BOOL)printDialog withProperties:(SlackPrintSettings *)withProperties;  // Print an object.
- (void) quitSaving:(SlackSavo)saving;  // Quit an application.

@end

// A color.
@interface SlackColor : SlackItem


@end

// A document.
@interface SlackDocument : SlackItem

@property (readonly) BOOL modified;  // Has the document been modified since the last save?
@property (copy) NSString *name;  // The document's name.
@property (copy) NSString *path;  // The document's path.


@end

// A window.
@interface SlackWindow : SlackItem

@property NSRect bounds;  // The bounding rectangle of the window.
@property (readonly) BOOL closeable;  // Whether the window has a close box.
@property (copy, readonly) SlackDocument *document;  // The document whose contents are being displayed in the window.
@property (readonly) BOOL floating;  // Whether the window floats.
- (NSInteger) id;  // The unique identifier of the window.
@property NSInteger index;  // The index of the window, ordered front to back.
@property (readonly) BOOL miniaturizable;  // Whether the window can be miniaturized.
@property BOOL miniaturized;  // Whether the window is currently miniaturized.
@property (readonly) BOOL modal;  // Whether the window is the application's current modal window.
@property (copy) NSString *name;  // The full title of the window.
@property (readonly) BOOL resizable;  // Whether the window can be resized.
@property (readonly) BOOL titled;  // Whether the window has a title bar.
@property BOOL visible;  // Whether the window is currently visible.
@property (readonly) BOOL zoomable;  // Whether the window can be zoomed.
@property BOOL zoomed;  // Whether the window is currently zoomed.


@end



/*
 * Text Suite
 */

// This subdivides the text into chunks that all have the same attributes.
@interface SlackAttributeRun : SlackItem

- (SBElementArray<SlackAttachment *> *) attachments;
- (SBElementArray<SlackAttributeRun *> *) attributeRuns;
- (SBElementArray<SlackCharacter *> *) characters;
- (SBElementArray<SlackParagraph *> *) paragraphs;
- (SBElementArray<SlackWord *> *) words;

@property (copy) NSColor *color;  // The color of the first character.
@property (copy) NSString *font;  // The name of the font of the first character.
@property NSInteger size;  // The size in points of the first character.


@end

// This subdivides the text into characters.
@interface SlackCharacter : SlackItem

- (SBElementArray<SlackAttachment *> *) attachments;
- (SBElementArray<SlackAttributeRun *> *) attributeRuns;
- (SBElementArray<SlackCharacter *> *) characters;
- (SBElementArray<SlackParagraph *> *) paragraphs;
- (SBElementArray<SlackWord *> *) words;

@property (copy) NSColor *color;  // The color of the first character.
@property (copy) NSString *font;  // The name of the font of the first character.
@property NSInteger size;  // The size in points of the first character.


@end

// This subdivides the text into paragraphs.
@interface SlackParagraph : SlackItem

- (SBElementArray<SlackAttachment *> *) attachments;
- (SBElementArray<SlackAttributeRun *> *) attributeRuns;
- (SBElementArray<SlackCharacter *> *) characters;
- (SBElementArray<SlackParagraph *> *) paragraphs;
- (SBElementArray<SlackWord *> *) words;

@property (copy) NSColor *color;  // The color of the first character.
@property (copy) NSString *font;  // The name of the font of the first character.
@property NSInteger size;  // The size in points of the first character.


@end

// Rich (styled) text
@interface SlackText : SlackItem

- (SBElementArray<SlackAttachment *> *) attachments;
- (SBElementArray<SlackAttributeRun *> *) attributeRuns;
- (SBElementArray<SlackCharacter *> *) characters;
- (SBElementArray<SlackParagraph *> *) paragraphs;
- (SBElementArray<SlackWord *> *) words;

@property (copy) NSColor *color;  // The color of the first character.
@property (copy) NSString *font;  // The name of the font of the first character.
@property NSInteger size;  // The size in points of the first character.


@end

// Represents an inline text attachment.  This class is used mainly for make commands.
@interface SlackAttachment : SlackText

@property (copy) NSString *fileName;  // The path to the file for the attachment


@end

// This subdivides the text into words.
@interface SlackWord : SlackItem

- (SBElementArray<SlackAttachment *> *) attachments;
- (SBElementArray<SlackAttributeRun *> *) attributeRuns;
- (SBElementArray<SlackCharacter *> *) characters;
- (SBElementArray<SlackParagraph *> *) paragraphs;
- (SBElementArray<SlackWord *> *) words;

@property (copy) NSColor *color;  // The color of the first character.
@property (copy) NSString *font;  // The name of the font of the first character.
@property NSInteger size;  // The size in points of the first character.


@end



/*
 * Type Definitions
 */

@interface SlackPrintSettings : SBObject <SlackGenericMethods>

@property NSInteger copies;  // the number of copies of a document to be printed
@property BOOL collating;  // Should printed copies be collated?
@property NSInteger startingPage;  // the first page of the document to be printed
@property NSInteger endingPage;  // the last page of the document to be printed
@property NSInteger pagesAcross;  // number of logical pages laid across a physical page
@property NSInteger pagesDown;  // number of logical pages laid out down a physical page
@property (copy) NSDate *requestedPrintTime;  // the time at which the desktop printer should print the document
@property SlackEnum errorHandling;  // how errors are handled
@property (copy) NSString *faxNumber;  // for fax number
@property (copy) NSString *targetPrinter;  // for target printer


@end

