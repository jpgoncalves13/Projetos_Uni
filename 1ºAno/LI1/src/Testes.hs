module Testes where 

import Types
import Tarefa4
import Tarefa1 
import Tarefa2
import Tarefa3
import Tarefa5
import Tarefa6

----------------------------------------------------Testes-Tarefa-1----------------------------------------------------------------
 
-- Just for visualize the maze in the terminal 
display :: Maze -> IO()
display m = putStrLn $ printMaze m 

-- Verify if the extremes Pieces of the Maze are correct
testGenerateMaze0 :: Maze -> Bool
testGenerateMaze0 m | even (length m) = corridorWalls (head m) && corridorWalls (last m) && evenfstlast m 
                    | otherwise       = corridorWalls (head m) && corridorWalls (last m) && oddfstlast m 

-- Verify if in a even Maze the corridors ,except the first and the last , have in the first position and in the last position an Wall and in the tunels an Enpty    
evenfstlast :: Maze -> Bool 
evenfstlast m | fstlastWalls1 (drop 1 (take (metade-1) m)) && fstlastWalls1 (init (drop (metade+1) m)) && tunelInMaze (take 2 (drop (metade-1) m)) = True 
              | otherwise                                                                                                                         = False
               where metade = div (length m) 2

-- Verify if in a odd Maze the corridors ,except the first and the last , have in the first position and in the last position an Wall and in the tunels an Enpty
oddfstlast :: Maze -> Bool 
oddfstlast m | fstlastWalls1 (drop 1 (take (metade) m)) && fstlastWalls1 (init (drop (metade+1) m)) && tunelInMaze (take 1 (drop (metade) m)) = True 
             | otherwise                                                                                                                     = False
               where metade = div (length m) 2

-- Verify if the corridors in one list are an list of Pieces Wall
fstlastWalls1 :: [Corridor] -> Bool 
fstlastWalls1 (x:[]) | head x == Wall && last x == Wall = True
                     | otherwise                        = False
fstlastWalls1 (x:xs) | head x == Wall && last x == Wall = fstlastWalls1 xs
                     | otherwise                        = False

-- Verify if the corridors in one list have in the first and in the last postition in all are Empty
tunelInMaze :: [Corridor] -> Bool 
tunelInMaze (x:[]) | head x == Empty && last x == Empty = True
                   | otherwise                        = False
tunelInMaze (x:xs) | head x == Empty && last x == Empty = tunelInMaze xs
                   | otherwise                        = False

-- Verify if the corridor is a list of Pieces Wall 
corridorWalls :: Corridor -> Bool
corridorWalls (x:[]) | x == Wall = True
                     | otherwise = False
corridorWalls (x:xs) | x == Wall = corridorWalls xs 
                     | otherwise = False    

-- Compare two mazes, if are equal or not 
testGenerateMaze1 :: Maze -> Maze -> Bool
testGenerateMaze1 (x:[]) (y:[]) | x == y    = True
                                | otherwise = False
testGenerateMaze1 (x:xs) (y:ys) | x == y    = testGenerateMaze1 xs ys 
                                | otherwise = False   


testMaze1 = generateMaze 15 10 1
testMaze2 = generateMaze 16 10 5
testMaze3 = generateMaze 15 11 7
testMaze4 = generateMaze 26 11 3
testMaze5 = generateMaze 150 50 9

{- Tests : 
(testGenerateMaze1 testMaze1 maze1) == True
(testGenerateMaze1 testMaze2 maze2) == True
(testGenerateMaze1 testMaze3 maze3) == True

(testGenerateMaze0 testMaze1) == True
(testGenerateMaze0 testMaze2) == True 
(testGenerateMaze0 testMaze3) == True 
-}

maze1 :: Maze
maze1 = [
         [Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall],
         [Wall,Food Little,Food Little,Wall,Food Little,Wall,Food Little,Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Wall,Wall],
         [Wall,Food Little,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Food Little,Wall],
         [Wall,Food Little,Empty,Wall,Wall,Wall,Empty,Empty,Empty,Wall,Wall,Wall,Empty,Food Little,Wall],
         [Empty,Food Little,Empty,Wall,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Wall,Empty,Food Little,Empty],
         [Empty,Food Little,Empty,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Empty,Food Little,Empty],
         [Wall,Food Little,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Food Little,Wall],
         [Wall,Food Little,Food Little,Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Wall,Food Little,Wall,Wall],
         [Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Wall,Wall,Wall,Food Little,Food Little,Wall],
         [Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall]
        ] 

maze2 :: Maze 
maze2 = [
         [Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall],
         [Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Wall,Wall,Food Little,Wall,Food Little,Wall,Wall,Wall,Food Little,Wall],
         [Wall,Food Little,Wall,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Wall,Food Little,Wall],
         [Wall,Food Big,Food Little,Empty,Wall,Wall,Wall,Empty,Empty,Wall,Wall,Wall,Empty,Food Little,Food Little,Wall],
         [Empty,Food Little,Food Little,Empty,Wall,Empty,Empty,Empty,Empty,Empty,Empty,Wall,Empty,Food Little,Food Little,Empty],
         [Empty,Food Little,Food Little,Empty,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Empty,Food Little,Food Little,Empty],
         [Wall,Food Little,Wall,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Food Little,Food Little,Wall],
         [Wall,Wall,Food Little,Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Wall,Food Little,Food Little,Wall],
         [Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Wall,Food Little,Food Little,Wall,Food Little,Food Little,Food Little,Food Little,Wall],
         [Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall]
        ]

maze3 :: Maze 
maze3 = [
         [Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall],
         [Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Wall,Food Little,Food Little,Food Little,Food Little,Wall],
         [Wall,Food Little,Food Little,Food Little,Wall,Food Little,Food Little,Wall,Wall,Wall,Food Little,Food Little,Food Little,Food Little,Wall],
         [Wall,Food Little,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Food Little,Wall],
         [Wall,Food Little,Empty,Wall,Wall,Wall,Empty,Empty,Empty,Wall,Wall,Wall,Empty,Food Little,Wall],
         [Empty,Food Little,Empty,Wall,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Wall,Empty,Food Little,Empty],
         [Wall,Food Little,Empty,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Empty,Food Little,Wall],
         [Wall,Food Little,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Food Little,Wall],
         [Wall,Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Food Little,Wall,Food Little,Food Little,Food Little,Food Little,Wall,Wall],
         [Wall,Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Wall,Wall,Food Little,Food Little,Food Little,Food Little,Food Little,Wall],
         [Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall]
        ]


----------------------------------------------------Testes-Tarefa-2----------------------------------------------------------------

mazeex1 = generateMaze 20 15 333
mazeex2 = generateMaze 40 18 555
mazeex3 = generateMaze 25 30 4343
mazeex4 = generateMaze 17 23 3443


stateex1 = State mazeex1 ljg 1 
stateex2 = State mazeex2 ljg 1
stateex3 = State mazeex3 ljg 1
stateex4 = State mazeex4 ljg 1


ljg = [jogador3,jogador4,jogador5]
jogador3 = (Pacman (PacState (1,(7,15),1,R,0,1) 0 Open Normal))
jogador4 = (Ghost  (GhoState (0,(4,12),1,R,0,1) Alive))
jogador5 = (Ghost (GhoState (2,(10,5),0,L,0,1) Alive))


testPlay1 = play (Move 1 R) (State mazeex1 ljg 1) 
testPlay2 = play (Move 1 L) (State mazeex2 ljg 1)
testPlay3 = play (Move 1 U) (State mazeex3 ljg 1)
testPlay4 = play (Move 1 D) (State mazeex4 ljg 1)


convertToList :: State -> [Player]
convertToList (State m l lv) = l


resultadoPlay1 = [(1,(2,12),1.0,R,1,1),(0,(5,12),1.0,R,0,1),(2,(10,5),0.0,L,0,1)]
resultadoPlay2 = [(1,(2,11),1.0,L,0,1),(0,(5,12),1.0,R,0,1),(2,(10,5),0.0,L,0,1)]
resultadoPlay3 = [(1,(2,11),1.0,U,0,1),(0,(5,12),1.0,R,0,1),(2,(10,5),0.0,L,0,1)]
resultadoPlay4 = [(1,(2,11),1.0,D,0,1),(0,(5,12),1.0,R,0,1),(2,(10,5),0.0,L,0,1)]


convertStaPla :: State -> [PlayerState]
convertStaPla s = convertToPlayerState $ convertToList s

convertToPlayerState :: [Player] -> [PlayerState] 
convertToPlayerState [] = []
convertToPlayerState (p1@(Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) = ((a,(x,y),b,c,d,e)) : convertToPlayerState t 
convertToPlayerState (p1@(Ghost (GhoState (a,(x,y),b,c,d,e) f)):t) = ((a,(x,y),b,c,d,e)) : convertToPlayerState t 


testeTarefa2 :: State -> State -> Bool
testeTarefa2 (State m x l) (State m1 y l1) | x==y = True
                                           | otherwise = False


testeLittleFood1 = [(1,(2,12),1.0,R,1,1),(0,(5,12),1.0,R,0,1),(2,(10,5),0.0,L,0,1)]
testeLittleFood2 = [(1,(2,11),1.0,L,0,1),(0,(5,12),1.0,R,0,1),(2,(10,5),0.0,L,0,1)]
testeLittleFood3 = [(1,(2,11),1.0,U,0,1),(0,(5,12),1.0,R,0,1),(2,(10,5),0.0,L,0,1)]
testeLittleFood4 = [(1,(2,11),1.0,D,0,1),(0,(5,12),1.0,R,0,1),(2,(10,5),0.0,L,0,1)]


ljg1 = [jogador6,jogador7,jogador8]
jogador6 = (Pacman (PacState (1,(4,10),1,R,0,1) 0 Open Mega))
jogador7 = (Ghost  (GhoState (0,(5,12),1,R,0,1) Dead))
jogador8 = (Ghost (GhoState (2,(10,5),0,L,0,1) Alive))


testPlay' = play (Move 1 R) (State mazeex2 ljg1 1)


testeFoodBig = [(1,(4,11),1.0,R,5,1),(0,(5,12),1.0,R,0,1),(2,(10,5),0.0,L,0,1)]


ljg2 = [jogador9,jogador10]
jogador9 = (Pacman (PacState (1,(4,10),1,R,0,1) 0 Open Mega))
jogador10 = (Ghost  (GhoState (0,(4,11),1,L,0,1) Dead))


testPlay'' = play (Move 1 R) (State mazeex2 ljg2 1)


testecomeFantasma = [(1,(4,11),1.0,R,10,1),(0,(8,19),1.0,L,0,1)]


----------------------------------------------------Testes-Tarefa-3----------------------------------------------------------------

{-
testMaze1 = generateMaze 15 10 1
testMaze2 = generateMaze 16 10 5
testMaze3 = generateMaze 15 11 7
testMaze4 = generateMaze 26 11 3
testMaze5 = generateMaze 150 50 9
-}

testInstructions1 = compactMaze testMaze1
testInstructions2 = compactMaze testMaze2
testInstructions3 = compactMaze testMaze3
testInstructions4 = compactMaze testMaze4

{- Tests : 
(testCompactMaze testInstructions1 instructions1) == True
(testCompactMaze testInstructions2 instructions2) == True
(testCompactMaze testInstructions3 instructions3) == True
(testCompactMaze testInstructions4 instructions4) == True
-}

-- Compare two Instructions
testCompactMaze :: Instructions -> Instructions -> Bool 
testCompactMaze (x:[]) (y:[]) | x == y    = True
                              | otherwise = False
testCompactMaze (x:xs) (y:ys) | x == y    = testCompactMaze xs ys
                              | otherwise = False

instructions1 :: Instructions
instructions1 = [
                 Instruct [(15,Wall)],
                 Instruct [(1,Wall),(2,Food Little),(1,Wall),(1,Food Little),(1,Wall),(1,Food Little),(1,Wall),(5,Food Little),(2,Wall)],
                 Instruct [(1,Wall),(1,Food Little),(11,Empty),(1,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(1,Food Little),(1,Empty),(3,Wall),(3,Empty),(3,Wall),(1,Empty),(1,Food Little),(1,Wall)],
                 Instruct [(1,Empty),(1,Food Little),(1,Empty),(1,Wall),(7,Empty),(1,Wall),(1,Empty),(1,Food Little),(1,Empty)],
                 Instruct [(1,Empty),(1,Food Little),(1,Empty),(9,Wall),(1,Empty),(1,Food Little),(1,Empty)],
                 Repeat 2,
                 Instruct [(1,Wall),(2,Food Little),(1,Wall),(7,Food Little),(1,Wall),(1,Food Little),(2,Wall)],
                 Instruct [(1,Wall),(8,Food Little),(3,Wall),(2,Food Little),(1,Wall)],
                 Repeat 0
                ]

instructions2 :: Instructions
instructions2 = [
                 Instruct [(16,Wall)],
                 Instruct [(1,Wall),(5,Food Little),(2,Wall),(1,Food Little),(1,Wall),(1,Food Little),(3,Wall),(1,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(1,Food Little),(1,Wall),(10,Empty),(1,Wall),(1,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(1,Food Big),(1,Food Little),(1,Empty),(3,Wall),(2,Empty),(3,Wall),(1,Empty),(2,Food Little),(1,Wall)],
                 Instruct [(1,Empty),(2,Food Little),(1,Empty),(1,Wall),(6,Empty),(1,Wall),(1,Empty),(2,Food Little),(1,Empty)],
                 Instruct [(1,Empty),(2,Food Little),(1,Empty),(8,Wall),(1,Empty),(2,Food Little),(1,Empty)],
                 Instruct [(1,Wall),(1,Food Little),(1,Wall),(10,Empty),(2,Food Little),(1,Wall)],
                 Instruct [(2,Wall),(1,Food Little),(1,Wall),(8,Food Little),(1,Wall),(2,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(6,Food Little),(1,Wall),(2,Food Little),(1,Wall),(4,Food Little),(1,Wall)],
                 Repeat 0
                ]

instructions3 :: Instructions
instructions3 = [
                 Instruct [(15,Wall)],
                 Instruct [(1,Wall),(8,Food Little),(1,Wall),(4,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(3,Food Little),(1,Wall),(2,Food Little),(3,Wall),(4,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(1,Food Little),(11,Empty),(1,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(1,Food Little),(1,Empty),(3,Wall),(3,Empty),(3,Wall),(1,Empty),(1,Food Little),(1,Wall)],
                 Instruct [(1,Empty),(1,Food Little),(1,Empty),(1,Wall),(7,Empty),(1,Wall),(1,Empty),(1,Food Little),(1,Empty)],
                 Instruct [(1,Wall),(1,Food Little),(1,Empty),(9,Wall),(1,Empty),(1,Food Little),(1,Wall)],
                 Repeat 3,
                 Instruct [(2,Wall),(6,Food Little),(1,Wall),(4,Food Little),(2,Wall)],
                 Instruct [(2,Wall),(5,Food Little),(2,Wall),(5,Food Little),(1,Wall)],
                 Repeat 0
                ]

instructions4 :: Instructions
instructions4 = [
                 Instruct [(26,Wall)],
                 Instruct [(1,Wall),(1,Food Little),(1,Wall),(5,Food Little),(1,Wall),(16,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(1,Food Little),(1,Wall),(1,Food Little),(1,Wall),(1,Food Little),(1,Wall),(5,Food Little),(1,Wall),(5,Food Little),(1,Wall),(1,Food Little),(2,Wall),(3,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(5,Food Little),(1,Wall),(1,Food Little),(10,Empty),(7,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(4,Food Little),(1,Wall),(2,Food Little),(1,Empty),(3,Wall),(2,Empty),(3,Wall),(1,Empty),(7,Food Little),(1,Wall)],
                 Instruct [(1,Empty),(1,Wall),(1,Food Big),(3,Food Little),(1,Wall),(1,Food Big),(1,Empty),(1,Wall),(6,Empty),(1,Wall),(1,Empty),(5,Food Little),(1,Wall),(1,Food Little),(1,Empty)],
                 Instruct [(1,Wall),(3,Food Little),(1,Wall),(3,Food Little),(1,Empty),(8,Wall),(1,Empty),(2,Food Little),(1,Wall),(2,Food Little),(1,Food Big),(2,Wall)],
                 Instruct [(1,Wall),(3,Food Little),(1,Wall),(3,Food Little),(10,Empty),(2,Food Little),(2,Wall),(3,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(4,Food Little),(1,Wall),(5,Food Little),(1,Wall),(2,Food Little),(1,Wall),(1,Food Little),(1,Wall),(2,Food Little),(1,Wall),(3,Food Little),(1,Wall),(1,Food Little),(1,Wall)],
                 Instruct [(1,Wall),(2,Food Little),(3,Wall),(2,Food Little),(1,Food Big),(2,Food Little),(1,Wall),(11,Food Little),(1,Wall),(1,Food Little),(1,Wall)],
                 Repeat 0
                ]