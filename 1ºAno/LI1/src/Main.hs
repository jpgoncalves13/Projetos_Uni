module Main where

import Data.Time.Clock.POSIX
import Control.Monad.IO.Class
import UI.NCurses
import Types
import FileUtils
import Tarefa4
import Tarefa5
import Tarefa6
import Testes


loadManager :: Manager
loadManager = ( Manager (loadMaze "maps/1.txt") 0 0 0 0 defaultDelayTime )

updateControlledPlayer :: Key -> Manager -> Manager
updateControlledPlayer k w@(Manager s@(State m ljg l) pid step before delta delay) | k == KeyUpArrow    = Manager (State m (rodaPac s ljg k) l) pid (step+1) before delta delay
                                                                                   | k == KeyDownArrow  = Manager (State m (rodaPac s ljg k) l) pid (step+1) before delta delay
                                                                                   | k == KeyLeftArrow  = Manager (State m (rodaPac s ljg k) l) pid (step+1) before delta delay
                                                                                   | k == KeyRightArrow = Manager (State m (rodaPac s ljg k) l) pid (step+1) before delta delay
                                                                                   | otherwise          = w

rodaPac :: State -> [Player] -> Key -> [Player]
rodaPac _ [] _ = []
rodaPac s l k | k == KeyUpArrow = mudaCoordsU (encontraPac l) : findGhosts l
              | k == KeyDownArrow = mudaCoordsD (encontraPac l) : findGhosts l
              | k == KeyLeftArrow = mudaCoordsL (encontraPac l) : findGhosts l 
              | k == KeyRightArrow = mudaCoordsR (encontraPac l) : findGhosts l

findGhosts :: [Player] -> [Player]
findGhosts [] = []
findGhosts ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) = t
findGhosts (p1@(Ghost (GhoState (a,(x,y),b,c,d,e) f)):ys) = p1 : findGhosts ys 

mudaCoordsU :: Player -> Player
mudaCoordsU (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) = (Pacman (PacState (a,(x,y),b,U,d,e) f g h))

mudaCoordsD :: Player -> Player
mudaCoordsD (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) = (Pacman (PacState (a,(x,y),b,D,d,e) f g h))

mudaCoordsL :: Player -> Player
mudaCoordsL (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) = (Pacman (PacState (a,(x,y),b,L,d,e) f g h))

mudaCoordsR :: Player -> Player
mudaCoordsR (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) = (Pacman (PacState (a,(x,y),b,R,d,e) f g h))


updateScreen :: Window  -> ColorID -> Manager -> Curses ()
updateScreen w a man =
                  do
                    updateWindow w $ do
                      clear
                      setColor a
                      moveCursor 0 0 
                      drawString $ show (state man)
                    render
     
currentTime :: IO Integer
currentTime = fmap ( round . (* 1000) ) getPOSIXTime

nextFrame :: Integer -> Manager -> Manager
nextFrame now man =  let update = (resetTimer now man)
                     in update {state = (passTime (step man) (state man))}

updateTime :: Integer -> Manager -> Manager
updateTime now man = man {delta = (now - before man)} 

resetTimer :: Integer -> Manager -> Manager
resetTimer now man = man {delta = 0, before = now} 


loop :: Window -> Manager -> Curses ()
loop w man@(Manager s pid step bf delt del ) = do 
  color_schema <- newColorID ColorBlue ColorWhite  10
  now <- liftIO  currentTime
  updateScreen w  color_schema man
  if ( delt > del )
    then loop w $ nextFrame now man
    else do
          ev <- getEvent w $ Just 0
          case ev of
                Nothing -> loop w (updateTime now man)
                Just (EventSpecialKey arrow ) -> loop w $ updateControlledPlayer arrow (updateTime now man)
                Just ev' ->
                  if (ev' == EventCharacter 'q')
                    then return ()
                    else loop w (updateTime now man)

main :: IO ()
main =
  runCurses $ do
    setEcho False
    setCursorMode CursorInvisible
    w <- defaultWindow
    loop w loadManager


