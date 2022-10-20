//
//  MachineInfo.swift
//  Today
//
//  Created by Masanori Kawai on 2022-10-20.
//

import Foundation

class MachineInfo {
    
    static func getUnameInfo() -> String
    {
        let unameChars:UnsafeMutablePointer<CChar> = UnsafeMutablePointer<CChar>.allocate(capacity: 1)
        getMachineString(unameChars)
        let result = String(cString: unameChars)
        unameChars.deallocate()
        return result
    }
}
