//
//  ViewController.swift
//  SampleUI
//
//  Created by Masanori Kawai on 2022-10-21.
//

import UIKit
import OSLog

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
    }

    @IBAction func btnClicked()
    {
        let log = Logger()
        log.info("didFinishLaunching")
        let machine:String = MachineInfo.getUnameInfo()
        log.info("info: \(machine)")
    }

}

