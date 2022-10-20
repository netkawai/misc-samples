//
//  AppDelegate.swift
//  SampleUI
//
//  Created by Masanori Kawai on 2022-10-21.
//

import UIKit
import Sentry

@main
class AppDelegate: UIResponder, UIApplicationDelegate {



    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // Override point for customization after application launch.

        SentrySDK.start { options in
                options.dsn = "https://c9a230fe605047c7b699b52c8aa5d284@o4504017904533504.ingest.sentry.io/4504017906040832"
                options.debug = true // Enabled debug when first installing is always helpful

                // Set tracesSampleRate to 1.0 to capture 100% of transactions for performance monitoring.
                // We recommend adjusting this value in production.
                options.tracesSampleRate = 1.0

                // Features turned off by default, but worth checking out
                options.enableAppHangTracking = true
                options.enableFileIOTracking = true
                options.enableCoreDataTracking = true
            }

        return true
    }

    // MARK: UISceneSession Lifecycle

    func application(_ application: UIApplication, configurationForConnecting connectingSceneSession: UISceneSession, options: UIScene.ConnectionOptions) -> UISceneConfiguration {
        // Called when a new scene session is being created.
        // Use this method to select a configuration to create the new scene with.
        return UISceneConfiguration(name: "Default Configuration", sessionRole: connectingSceneSession.role)
    }

    func application(_ application: UIApplication, didDiscardSceneSessions sceneSessions: Set<UISceneSession>) {
        // Called when the user discards a scene session.
        // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
        // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
    }


}

