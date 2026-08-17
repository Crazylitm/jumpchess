// 统一鼠标控制助手：click / move / activate 三个子命令。
// 由 Python 预编译成二进制后直接调用，避免每次落子现场编译 Swift（原 swift -e 方式，
// 549 手要 13+ 分钟的元凶）。
// 用法：
//   jumpctl click <x> <y>
//   jumpctl move  <x1> <y1> <x2> <y2>
//   jumpctl activate
import CoreGraphics
import Foundation
import AppKit

let args = CommandLine.arguments
guard args.count >= 2 else { exit(1) }
let cmd = args[1]
let src = CGEventSource(stateID: .hidSystemState)

func click(_ x: Double, _ y: Double) {
    let pt = CGPoint(x: x, y: y)
    let down = CGEvent(mouseEventSource: src, mouseType: .leftMouseDown,
                       mouseCursorPosition: pt, mouseButton: .left)
    let up = CGEvent(mouseEventSource: src, mouseType: .leftMouseUp,
                     mouseCursorPosition: pt, mouseButton: .left)
    down?.post(tap: .cghidEventTap)
    usleep(60_000)
    up?.post(tap: .cghidEventTap)
}

func move(_ x1: Double, _ y1: Double, _ x2: Double, _ y2: Double) {
    let steps = 16
    for i in 0..<steps {
        let t = Double(i) / Double(steps - 1)
        let pt = CGPoint(x: x1 + t * (x2 - x1), y: y1 + t * (y2 - y1))
        let ev = CGEvent(mouseEventSource: src, mouseType: .mouseMoved,
                         mouseCursorPosition: pt, mouseButton: .left)
        ev?.post(tap: .cghidEventTap)
        usleep(18_000)
    }
}

func activate() {
    for app in NSWorkspace.shared.runningApplications where app.localizedName == "jump0" {
        app.activate(options: [.activateIgnoringOtherApps])
    }
}

switch cmd {
case "click":
    guard args.count >= 4, let x = Double(args[2]), let y = Double(args[3]) else { exit(1) }
    click(x, y)
case "move":
    guard args.count >= 6,
          let x1 = Double(args[2]), let y1 = Double(args[3]),
          let x2 = Double(args[4]), let y2 = Double(args[5]) else { exit(1) }
    move(x1, y1, x2, y2)
case "activate":
    activate()
default:
    exit(1)
}
