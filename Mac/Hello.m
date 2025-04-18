/* clang -o hello Hello.m -framework Appkit */
#include <Cocoa/Cocoa.h>

@interface MyWindow : NSWindow {
  NSTextField* label;
}
- (instancetype)init;
- (BOOL)windowShouldClose:(id)sender;
@end

@implementation MyWindow
- (instancetype)init {
  label = [[[NSTextField alloc] initWithFrame:NSMakeRect(5, 100, 290, 100)] autorelease];
  [label setStringValue:@"Hello, World!"];
  [label setBezeled:NO];
  [label setDrawsBackground:NO];
  [label setEditable:NO];
  [label setSelectable:NO];
  // Objective-C 1.0
  //[label setTextColor:[NSColor colorWithSRGBRed:0.0 green:0.5 blue:0.0 alpha:1.0]];
  // Objective-C 2.0
  label.textColor = [NSColor colorWithSRGBRed:0.0 green:0.5 blue:0.0 alpha:1.0];
  [label setFont:[[NSFontManager sharedFontManager] convertFont:[[NSFontManager sharedFontManager] convertFont:[NSFont fontWithName:[[label font] fontName] size:45] toHaveTrait:NSFontBoldTrait] toHaveTrait:NSFontItalicTrait]];

  [super initWithContentRect:NSMakeRect(0, 0, 300, 300) styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:NO];
  [self setTitle:@"Hello world (label)"];
  [[self contentView] addSubview:label];
  [self center];
  [self setIsVisible:YES];
  return self;
}

- (BOOL)windowShouldClose:(id)sender {
  [NSApp terminate:sender];
  return YES;
}
@end

int main(int argc, char* argv[]) {
  [NSApplication sharedApplication];
  [[[[MyWindow alloc] init] autorelease] makeMainWindow];
  [NSApp run];
}