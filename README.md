TravelingSoot
==============================================================================

Have a ball!

At this time this plugin is being used only in closed alpha testing. Unless 
that's you, this probably won't yet make much sense or be of much use.

Requirements
------------------------------------------------------------------------------
### Target Project ###

1.  Using Unreal Engine 4 (UE4) **_compiled from source_**.
    
    At this time we are only testing against version [4.26.2-release][UE4
    4.26.2-release]; other versions of UE may or may not work.
    
    The `README.md` file has instructions, available [online][UE4 README] or in
    the root directory of the Git repository and release archive.

    [UE4 4.26.2-release]: https://github.com/EpicGames/UnrealEngine/releases/tag/4.26.2-release
    [UE4 README]: https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/README.md#getting-up-and-running
    
2.  Using [PlasticSCM][] for version control.
    
    [PlasticSCM]: https://www.plasticscm.com
    
3.  Windows environment able to build UE4 project for Linux server target.
    
    Details TODO.
    
4.  Some Linux to run the server on.

Setup
------------------------------------------------------------------------------

> ### 📢 NOTE ###
> 
> 1.  _Unix-style_ paths (`/`-separated) and shell syntax (`bash`) are used in
>     these instructions.
>     
>     If you are running Window you will need to adjust path and shell command
>     formats.
>     
> 2.  The `${PROJECT}` variable is used to denote the root directory of the target
>     UE4 project (where the `.uproject` file lives).
>     
>     Set `${PROJECT}` to your project root or replace it with the actual path.
>     

1.  Ensure that your project has a `${PROJECT}/Plugins` directory that is
    checked-in to your PlasticSCM repository.
    
    If you do not already have a `${PROJECT}/Plugins` directory, create one and 
    check it in.
    
2.  In the project workspace view of the PlasticSCM GUI right-click on the
    `Plugins` directory and select _Create XLink_.
    
3.  Fill in:
    
    |       Field       |         Value          |
    | ----------------- | ---------------------- |
    | Target server     | `futureperfect@cloud`  |
    | Target repository | `TravelingSoot`        |
    | Target changeset  | _(select most recent)_ |
    | Xlink name        | `TravelingSoot`        |
    
    and click _Create_.

4.  Commit the new XLink and update the workspace.
    
5.  Edit project config files:
    
    1.  Edit `${PROJECT}/Config/DefaultEngine.ini`
        
        1.  `[/Script/EngineSettings.GameMapsSettings]` Section
            
            Add:
            
            ```ini
            GameDefaultMap=/TravelingSoot/Maps/SootEntry.SootEntry
            GlobalDefaultGameMode=/Script/TravelingSoot.SootGameMode
            ServerDefaultMap=${MAP_YOU_WANT_LOAD}
            ```
            
            where `${MAP_YOU_WANT_LOAD}` is the path-like-thing that you want the
            server to drop people into when they connect.
            
        2.  `[/Script/Engine.Engine]` Section
            
            If your project was created from a template like `TP_Blank` you
            may need to add entries like:
            
            ```ini
            +ActiveClassRedirects=(OldClassName="TP_BlankBPCharacter",NewClassName="SootCharacter")
            +ActiveClassRedirects=(OldClassName="TP_BlankBPGameMode",NewClassName="SootGameMode")
            ```
            
        3.  `[/TravelingSoot/Maps/SootEntry.SootEntry_C]` Section
            
            Add section and value:
            
            ```ini
            Endpoint=${HOST}:${PORT}
            ```
            
            where `${HOST}` is the IP address or host/DNS name of your server
            and `${PORT}` is the port the game server will run on.
            
            ### Examples ####
            
            1.  Using an IP address and the default UE port `7777`:
                
                ```ini
                [/TravelingSoot/Maps/SootEntry.SootEntry_C]
                Endpoint=10.0.0.1:7777
                ```
                
            2.  Using a DNS name and a custom port:
                
                ```ini
                [/TravelingSoot/Maps/SootEntry.SootEntry_C]
                Endpoint=whatever.expand.live:7654
                ```
                
            > ### 📢 NOTE ###
            > 
            > If you change this value later, you need to re-compile
            > the `SootEntry` map' level blueprint to see the changes.
            
    2.  Edit `${PROJECT}/Config/DefaultGame.ini`
        
        1.  `[/Script/UnrealEd.ProjectPackagingSettings]` Section
            
            Add:
            
            ```ini
            +MapsToCook=(FilePath="/TravelingSoot/Maps/SootEntry")
            +MapsToCook=(FilePath="<MAP_YOU_WANT_LOAD>")
            ```
            
            Note that in _this_ case, the syntax uses quotes and what look like
            file paths , opposed to the namespace-like (?) values used in
            `${PROJECT}/Config/DefaultEngine.ini`.
            
            The difference is seen in
            
            | File  |    `${PROJECT}/Config/DefaultEngine.ini`    | `${PROJECT}/Config/DefaultGame.ini` |
            | ----- | ----------------------------------------- | --------------------------------- |
            | Value | `/TravelingSoot/Maps/SootEntry.SootEntry` | `"/TravelingSoot/Maps/SootEntry"` |
      
      3.  Edit `${PROJECT}/Config/DefaultInput.ini`
          
          1.  `[/Script/Engine.InputSettings]` Section
              
              Add a new _ActionMappings_ entry:
              
                  +ActionMappings=(ActionName="Jump",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=SpaceBar)
              
              and several new _AxisMappings_ entries:
              
                  +AxisMappings=(AxisName="MoveForward",Scale=1.000000,Key=W)
                  +AxisMappings=(AxisName="MoveForward",Scale=-1.000000,Key=S)
                  +AxisMappings=(AxisName="MoveForward",Scale=1.000000,Key=Up)
                  +AxisMappings=(AxisName="MoveForward",Scale=-1.000000,Key=Down)
                  +AxisMappings=(AxisName="MoveRight",Scale=-1.000000,Key=A)
                  +AxisMappings=(AxisName="MoveRight",Scale=1.000000,Key=D)
                  +AxisMappings=(AxisName="TurnRate",Scale=-1.000000,Key=Left)
                  +AxisMappings=(AxisName="TurnRate",Scale=1.000000,Key=Right)
                  +AxisMappings=(AxisName="Turn",Scale=1.000000,Key=MouseX)
                  +AxisMappings=(AxisName="LookUp",Scale=-1.000000,Key=MouseY)
          
            
  6.  Edit `.Target.cs` files:
      
      1.  Edit `${PROJECT}/Source/${NAME}.target.cs`
          
          Change
          
          ```cs
          ExtraModuleNames.AddRange( new string[] { "$NAME" } );
          ```
          
          to
          
          ```cs
          ExtraModuleNames.AddRange( new string[] { "$NAME", "TravelingSoot" } );
          ```
          
      2.  Edit `${PROJECT}/Source/${NAME}Editor.target.cs`
          
          Change
          
          ```cs
          ExtraModuleNames.AddRange( new string[] { "$NAME" } );
          ```
          
          to
          
          ```cs
            ExtraModuleNames.AddRange( new string[] { "$NAME", "TravelingSoot" } );
          ``
          
      3.  Create `${PROJECT}/Source/${NAME}Server.target.cs`
          
          It should look like:
          
          ```cs
          using UnrealBuildTool;
          using System.Collections.Generic;

          public class ${NAME}ServerTarget : TargetRules
          {
            public ${NAME}ServerTarget(TargetInfo Target) : base(Target)
            {
              Type = TargetType.Server;
              DefaultBuildSettings = BuildSettingsVersion.V2;
              ExtraModuleNames.AddRange( new string[] { "$NAME", "TravelingSoot" } );
            }
          }
          ```
        
7.  Re-generate you product's IDE files
    
    -   ### Windows / Visual Studio ###
        
        Right-click on the `.sln` file in your project root and choose
        _Generate Visual Studio project files.
        
        If you have more than one UE installation present, make sure you select
        the one built from `4.26.2-release` source code.
        
        ![Visual Studio Example](https://docs.unrealengine.com/Images/InteractiveExperiences/Networking/HowTo/DedicatedServers/GenerateProjectFiles.webp)
        
    -   ### macOS / XCode ###
        
        I think there is _supposed_ to be a way to regenerate XCode project
        files from the GUI (like in Windows), but I found it flaky and now do
        the regeneration from the terminal:
        
        ```shell
        ${UE_SRC}/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh \
            -project="${PROJECT}/${NAME}.uproject" \
            -game
        ```
        
        where `${UE_SRC}` is the path to the `4.26.2-release` source code.
        
        > ### 📢 NOTE ###
        > 
        > The `-project=` option must be given the _absolute_ path to your
        > `.uproject` file.

That should do it. If you're able to compile and package, things should work.
Let us know and we'll put it up.
