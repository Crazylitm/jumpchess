// Posts a left mouse click at (x, y) in screen coordinates.
// Usage: swift clk.swift <x> <y>
// Lives in the repo (not /tmp) so no other local process can replace it.
import CoreGraphics
import Foundation

let args = CommandLine.arguments
guard args.count >= 3, let x = Double(args[1]), let y = Double(args[2]) else {
    FileHandle.standardError.write("usage: clk.swift <x> <y>\n".data(using: .utf8)!)
    exit(1)
}

let pt = CGPoint(x: x, y: y)
let src = CGEventSource(stateID: .hidSystemState)
let down = CGEvent(mouseEventSource: src, mouseType: .leftMouseDown,
                   mouseCursorPosition: pt, mouseButton: .left)
let up = CGEvent(mouseEventSource: src, mouseType: .leftMouseUp,
                 mouseCursorPosition: pt, mouseButton: .left)
down?.post(tap: .cghidEventTap)
usleep(60_000)
up?.post(tap: .cghidEventTap)
