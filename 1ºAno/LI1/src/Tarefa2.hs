{- | Introdução
      Esta tarefa tem como objetivo efetuar jogadas para Players do tipo Pacman e para isso tivemos de implementar a função play.    
      
     Objetivos  
      Com o objetivo da função play em mente, tentamos fazer de uma maneira em que o código não ficasse muito complicado de se ler, logo tentamos fazer as funções por partes.
      Primeiramente, pensamos na movimentação dos Players, depois pensamos nas interações dos Players com as Pieces presentes no Maze (inclusivamente outros Players) e o que acontece logo a seguir (pontuação, reaparecimento dos Players, velocidade, modo dos Players, vidas restantes), e por fim o aparecimento dos Players do outro lado do Labirinto quando entram no tunel.
      Com estes objetivos que são necessários cumprir fizemos a função play que efetua jogadas para um jogador da lista de Players presente no State.

     Conclusão
      Em suma, os principais resultados que obtivemos nesta tarefa foi o de conseguir efetuar jogadas para um Player alterando o seu State.   -}

module Tarefa2 where

import Types

-- | Movement of Pacman given one Orientation and one Player
movPlayer :: Orientation -> Player -> Player
movPlayer ori (Pacman (PacState (a,(x,y),g,h,b,c) d e f)) | h==U && ori == U = (Pacman (PacState (a,(x-1,y),g,U,b,c) d e f))       
                                                          | h==D && ori == D = (Pacman (PacState (a,(x+1,y),g,D,b,c) d e f))  
                                                          | h==L && ori == L = (Pacman (PacState (a,(x,y-1),g,L,b,c) d e f)) 
                                                          | h==R && ori == R = (Pacman (PacState (a,(x,y+1),g,R,b,c) d e f))  
                                                          | h==Null && ori == U = (Pacman (PacState (a,(x,y),g,U,b,c) d e f))
                                                          | h==Null && ori == D = (Pacman (PacState (a,(x,y),g,D,b,c) d e f))
                                                          | h==Null && ori == L = (Pacman (PacState (a,(x,y),g,L,b,c) d e f))
                                                          | h==Null && ori == R = (Pacman (PacState (a,(x,y),g,R,b,c) d e f)) 
                                                          | h/=U && ori == U = (Pacman (PacState (a,(x,y),g,U,b,c) d e f))
                                                          | h/=D && ori == D = (Pacman (PacState (a,(x,y),g,D,b,c) d e f))
                                                          | h/=L && ori == L = (Pacman (PacState (a,(x,y),g,L,b,c) d e f))
                                                          | h/=R && ori == R = (Pacman (PacState (a,(x,y),g,R,b,c) d e f))
                                                          | ori == Null = (Pacman (PacState (a,(x,y),g,Null,b,c) d e f))         
movPlayer ori (Ghost (GhoState (a,(x,y),g,h,b,c) d)) | h==U && ori == U = (Ghost (GhoState (a,(x-1,y),g,U,b,c) d))       
                                                     | h==D && ori == D = (Ghost (GhoState (a,(x+1,y),g,D,b,c) d))  
                                                     | h==L && ori == L = (Ghost (GhoState (a,(x,y-1),g,L,b,c) d)) 
                                                     | h==R && ori == R = (Ghost (GhoState (a,(x,y+1),g,R,b,c) d)) 
                                                     | h==Null && ori == U = (Ghost (GhoState (a,(x,y),g,U,b,c) d))       
                                                     | h==Null && ori == D = (Ghost (GhoState (a,(x,y),g,D,b,c) d))  
                                                     | h==Null && ori == L = (Ghost (GhoState (a,(x,y),g,L,b,c) d)) 
                                                     | h==Null && ori == R = (Ghost (GhoState (a,(x,y),g,R,b,c) d)) 
                                                     | h/=U && ori == U = (Ghost (GhoState (a,(x,y),g,U,b,c) d))
                                                     | h/=D && ori == D = (Ghost (GhoState (a,(x,y),g,D,b,c) d))
                                                     | h/=L && ori == L = (Ghost (GhoState (a,(x,y),g,L,b,c) d))
                                                     | h/=R && ori == R = (Ghost (GhoState (a,(x,y),g,R,b,c) d))
                                                     | ori == Null = (Ghost (GhoState (a,(x,y),g,Null,b,c) d))


-- | Verify if one ID corresponds to the ID of a Player
encontraID :: Int -> Player -> Bool
encontraID z (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) | z == a    = True
                                                         | otherwise = False 
encontraID z (Ghost (GhoState (a,(x,y),b,c,d,e) f)) | z == a    = True
                                                    | otherwise = False       


-- | Verify if the ID correspond to a Player in a Player list, and move that Player
movPlayerId :: Int -> Orientation -> [Player] -> [Player] 
movPlayerId id ori (h:[]) | encontraID id h = [(movPlayer ori h)] 
                          | otherwise       = [h] 
movPlayerId id ori (h:t) | encontraID id h = (movPlayer ori h) : t 
                         | otherwise       = h : movPlayerId id ori t


-- | Identify the piece in a certain position of the Corridor
showPiece :: Maze -> Int -> Int -> Piece 
showPiece m a b = aux (aux m a) b                            
                where aux (x:[]) a = x
                      aux (x:xs) a | a == 0 = x
                                   | otherwise = aux xs (a-1)   


-- | Identify the next piece in the Maze, given a certain Orientation
whosPiece :: Maze -> Orientation -> Player -> [Player] -> Piece
whosPiece m ori (p1@(Pacman (PacState (a,(x,y),b,c,d,e) f g h))) (p2@(Ghost (GhoState (a1,(x1,y1),b1,c1,d1,e1) f1)):t) | ori == U && x1 == (x-1) && y == y1 = PacPlayer p2   
                                                                                                                       | ori == D && x1 == (x+1) && y == y1 = PacPlayer p2  
                                                                                                                       | ori == L && x == x1 && y1 == (y-1) = PacPlayer p2 
                                                                                                                       | ori == R && x == x1 && y1 == (y+1) = PacPlayer p2   
                                                                                                                       | otherwise                          = whosPiece m ori p1 t 
whosPiece m ori (p1@(Pacman (PacState (a,(x,y),b,c,d,e) f g h))) (p2@(Pacman (PacState (a1,(x1,y1),b1,c1,d1,e1) f1 g1 h1)):t) | ori == U && x1 == (x-1) && y == y1 = PacPlayer p2   
                                                                                                                              | ori == D && x1 == (x+1) && y == y1 = PacPlayer p2  
                                                                                                                              | ori == L && x == x1 && y1 == (y-1) = PacPlayer p2 
                                                                                                                              | ori == R && x == x1 && y1 == (y+1) = PacPlayer p2  
                                                                                                                              | otherwise                          = whosPiece m ori p1 t                                                                                                         
whosPiece m ori (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) _ | ori == U = showPiece m (x-1) y 
                                                              | ori == D = showPiece m (x+1) y 
                                                              | ori == L = showPiece m x (y-1) 
                                                              | ori == R = showPiece m x (y+1)
                                                              | ori == Null = showPiece m x y 
whosPiece m ori (p1@(Ghost (GhoState (a,(x,y),b,c,d,e) f))) (p2@(Pacman (PacState (a1,(x1,y1),b1,c1,d1,e1) f1 g1 h1)):t) | ori == U && x1 == (x-1) && y == y1 = PacPlayer p2   
                                                                                                                         | ori == D && x1 == (x+1) && y == y1 = PacPlayer p2  
                                                                                                                         | ori == L && x == x1 && y1 == (y-1) = PacPlayer p2 
                                                                                                                         | ori == R && x == x1 && y1 == (y+1) = PacPlayer p2  
                                                                                                                         | otherwise                          = whosPiece m ori p1 t  
whosPiece m ori (p1@(Ghost (GhoState (a,(x,y),b,c,d,e) f))) (p2@(Ghost (GhoState (a1,(x1,y1),b1,c1,d1,e1) f1)):t) | ori == U && x1 == (x-1) && y == y1 = PacPlayer p2   
                                                                                                                  | ori == D && x1 == (x+1) && y == y1 = PacPlayer p2  
                                                                                                                  | ori == L && x == x1 && y1 == (y-1) = PacPlayer p2 
                                                                                                                  | ori == R && x == x1 && y1 == (y+1) = PacPlayer p2  
                                                                                                                  | otherwise                          = whosPiece m ori p1 t                                                                                            
whosPiece m ori (Ghost (GhoState (a,(x,y),b,c,d,e) f)) _ | ori == U = showPiece m (x-1) y   
                                                         | ori == D = showPiece m (x+1) y 
                                                         | ori == L = showPiece m x (y-1) 
                                                         | ori == R = showPiece m x (y+1)
                                                         | ori == Null = showPiece m x y 


-- | If the ID is found in one Player, that Player should gain one more point in the pontuation
eatLittle :: Int -> [Player] -> [Player]
eatLittle id [] = []
eatLittle id (h@(Pacman (PacState (a,(x,y),g,c,d,e) i m n)):t) | encontraID id h = (Pacman(PacState (a,(x,y),g,c,d+1,e) i m n)) : t
                                                               | otherwise       = h : eatLittle id t  
eatLittle id (h@(Ghost (GhoState (a,(x,y),g,c,d,e) i)):t) = h : eatLittle id t


-- | Changes the list of Players, depending on the Maze, the ID, the Orientation and a Piece 

movWhenEat :: Maze -> Int -> Orientation -> Piece -> [Player] -> [Player]
movWhenEat _ _ _ _ [] = []
movWhenEat _ _ Null _ s = s
movWhenEat m id ori Empty (p1@(Pacman (PacState (a,(x,y),g,c,d,e) i j n)):t) | encontraID id p1 = whenEmpty m ori (x,y) p1 : t 
                                                                             | otherwise        = p1 : movWhenEat m id ori Empty t 
movWhenEat m id ori Empty (p1@(Ghost (GhoState (a,(x,y),g,c,d,e) i)):t) | encontraID id p1 = whenEmpty m ori (x,y) p1 : t 
                                                                        | otherwise        = p1 : movWhenEat m id ori Empty t 
movWhenEat m id ori Wall (h:t) | encontraID id h = nextWall id ori (h:t) 
                               | otherwise       = h : movWhenEat m id ori Wall t   
movWhenEat m id ori (Food Little) (h:t) | encontraID id h = movPlayerId id ori (eatLittle id (h : t)) 
                                        | otherwise       = h : movWhenEat m id ori (Food Little) t
movWhenEat m id ori (Food Big) (h:t) | encontraID id h    = movPlayerId id ori (eatBig1 id (h:t))
                                     | otherwise          = movWhenEat m id ori (Food Big) (t++[h]) 
movWhenEat m id ori (PacPlayer x) (h:t) | encontraID id h    = nextPlayer ori m h x t
                                        | otherwise          = movWhenEat m id ori (PacPlayer x) (t++[h]) 


-- | If the ID is found in a Player of a list of Players, then its velocity should go to 0.0.

-- ^ Otherwise, the list of Players continue equal

nextWall :: Int -> Orientation -> [Player] -> [Player]
nextWall id ori [] = [] 
nextWall id ori (p1@(Pacman (PacState (a,(x,y),b,c,g,h) d e f)):t) | encontraID id p1 && ori == U = (Pacman (PacState (a,(x,y),b,U,g,h) d e f)) : t
                                                                   | encontraID id p1 && ori == D = (Pacman (PacState (a,(x,y),b,D,g,h) d e f)) : t
                                                                   | encontraID id p1 && ori == L = (Pacman (PacState (a,(x,y),b,L,g,h) d e f)) : t 
                                                                   | encontraID id p1 && ori == R = (Pacman (PacState (a,(x,y),b,R,g,h) d e f)) : t
                                                                   | encontraID id p1 && ori == Null = (Pacman (PacState (a,(x,y),b,Null,g,h) d e f)) : t
                                                                   | otherwise                    = p1 : nextWall id ori t 
nextWall id ori (p2@(Ghost (GhoState (a,(x,y),b,c,d,e) f)):t) | encontraID id p2 && ori == U = (Ghost (GhoState (a,(x,y),b,U,d,e) f)) :t
                                                              | encontraID id p2 && ori == D = (Ghost (GhoState (a,(x,y),b,D,d,e) f)) :t
                                                              | encontraID id p2 && ori == L = (Ghost (GhoState (a,(x,y),b,L,d,e) f)) :t
                                                              | encontraID id p2 && ori == R = (Ghost (GhoState (a,(x,y),b,R,d,e) f)) :t
                                                              | encontraID id p2 && ori == Null = ((Ghost (GhoState (a,(x,y),b,Null,d,e) f)):t)
                                                              | otherwise                    = p2 : nextWall id ori t

-- | Verify if the Player is in the right position to move to the next position depending on its Orientation
goeat :: Int -> Orientation -> [Player] -> Bool     
goeat id ori [] = False 
goeat id ori (h@(Ghost (GhoState (a,(x,y),v,d,b,c) n)):t) | ori == d && encontraID id h = True 
                                                          | otherwise                   = goeat id ori t 
goeat id ori (h@(Pacman (PacState (a,(x,y),g,c,d,e) i m n)):t) | ori == c && encontraID id h = True 
                                                               | otherwise                   = goeat id ori t                                                         


-- | The Player adds five to its pontuation
eatBig :: Player -> Player
eatBig (Pacman (PacState (a,(x,y),g,c,d,e) i m n)) = (Pacman (PacState (a,(x,y),1,c,d+5,e) (i+10) m Mega))


-- | The Player halves its velocity
movGhostDead :: Player -> Player
movGhostDead (Ghost (GhoState (a,(x,y),v,d,b,c) f)) = (Ghost (GhoState (a,(x,y),0.5,d,b,c) Dead))


-- | Given an ID and a list of Players, if the ID is found for a Pacman, that player should gain five points in its pontuation,
-- and the function ,movGhostDead should be applyed for all the Ghosts in the maze
eatBig1 :: Int -> [Player] -> [Player]
eatBig1 id [] = []
eatBig1 id (h@(Ghost (GhoState (a,(x,y),v,d,b,c) f)):t) = (movGhostDead h) : eatBig1 id t  
eatBig1 id (h@(Pacman (PacState (a,(x,y),g,c,d,e) i m n)):t) | encontraID id h = eatBig h : eatBig1 id t
                                                             | otherwise       = h : eatBig1 id t  


-- | The Player increases its pontuation by ten points
eatGhost :: Player -> Player
eatGhost (Pacman (PacState (a,(x,y),b,c,d,e) i m Mega)) = (Pacman (PacState (a,(x,y),1,c,d+10,e) i m Mega))


-- | If a Player Pacman encounters a Player Ghost, the list of Players will change
nextPlayer :: Orientation -> Maze -> Player -> Player -> [Player] -> [Player]
nextPlayer ori mz p1 p2 [] = [] 
nextPlayer ori mz ((p1@(Pacman (PacState (a,(x,y),b,c,d,e) f g Normal)))) ((p2@(Ghost (GhoState (k,(l,m),n,o,p,q) Alive)))) t | ori == c  = movPlayer ori (dyePac p1) : t
                                                                                                                              | otherwise = movPlayer ori p1 : t 
nextPlayer ori mz ((p1@(Pacman (PacState (a,(x,y),b,c,d,e) f g Mega)))) ((p2@(Ghost (GhoState (k,(l,m),n,o,p,q) Dead)))) t | ori == c  = movPlayer ori (eatGhost p1) : spawnGhost mz p2 t
                                                                                                                           | otherwise = movPlayer ori p1 : t 
nextPlayer ori mz ((p1@(Ghost (GhoState (a,(x,y),b,c,d,e) Alive)))) ((p2@(Pacman (PacState (k,(l,m),n,o,p,q) f g Normal)))) t = movPlayer ori p1 : t 
nextPlayer ori mz ((p1@(Ghost (GhoState (a,(x,y),b,c,d,e) Dead)))) ((p2@(Pacman (PacState (k,(l,m),n,o,p,q) f g Mega)))) t | ori == c  = (spawnGhost mz p1 t) ++ [movPlayer ori (eatGhost p1)]
                                                                                                                           | otherwise = movPlayer ori p1 : t 
nextPlayer ori mz (p1@(Ghost (GhoState (a,(x,y),b,c,d,e) f))) (p2@(Ghost (GhoState (a1,(x1,y1),b1,c1,d1,e1) f1)))  t = p1 : t
--nextPlayer ori mz ((p1@(Pacman (PacState (a,(x,y),b,c,d,e) f g h)))) _ t = movPlayer ori p1 : t 


-- | What happens to Pacman when he dies, if he has 0 lives or more
dyePac :: Player -> Player
dyePac (Pacman (PacState (a,(x,y),b,c,d,0) z f g)) = (Pacman (PacState (a,(x,y),b,c,d,0) z f Dying))
dyePac (Pacman (PacState (a,(x,y),b,c,d,e) z f g)) = (Pacman (PacState (a,(x,y),b,c,d,e-1) z f Normal))


-- | Choose the right Ghost of the list of Players to spawn in the Ghost house
spawnGhost :: Maze -> Player -> [Player] -> [Player] 
spawnGhost m p1 [] = []
spawnGhost m p1 (h:t) | p1 == h   = spawnGho m h : t 
                      | otherwise = h : spawnGhost m p1 t


-- | Defines the spawn of the Ghost when it is an even or odd heigth of the Maze
spawnGho :: Maze -> Player -> Player
spawnGho m (Ghost (GhoState (a,(x,y),b,c,d,q) z)) | even (length m) = Ghost (GhoState (a,((div (length m) 2) - 1 , (div (length (head m)) 2 ) - 1) ,1,c,d,1)  Alive)
                                                  | odd (length m)  = Ghost (GhoState (a,((div (length m) 2), (div (length (head m)) 2 )),1,c,d,1) Alive)  


-- | What happens to the Player when he moves to a Piece Empty
whenEmpty :: Maze -> Orientation -> Coords -> Player -> Player 
whenEmpty m ori (x,y) h | tunelEmpty ori m (x,y) = tunel m h 
                        | otherwise              = movPlayer ori h 


-- | Define if the piece Empty is the Tunnel or not and verifies if the Orientation is the correct
tunelEmpty :: Orientation -> Maze -> Coords -> Bool 
tunelEmpty ori m (x,y) | ori == L && y == 0 = True 
                       | ori == R && y == (length (head m)) - 1 && x == (div (length m) 2) = True 
                       | ori == R && y == (length (head m)) - 1 && x == (div (length m) 2) - 1 = True 
                       | ori == Null && y == 0 = False 
                       | ori == Null && y == (length (head m)) - 1 && x == (div (length m) 2) - 1 = False
                       | ori == Null && y == (length (head m)) - 1 && x == (div (length m) 2) = False 
                       | otherwise = False  


-- | Change the position of the player when he enters the Tunnel
tunel :: Maze -> Player -> Player
tunel m (Pacman (PacState (a,(x,0),b,c,d,e) f g h)) = (Pacman (PacState (a,(x,y-1),b,L,d,e) f g h))
                                                    where y = length (head m) 
tunel m (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) = (Pacman (PacState (a,(x,0),b,R,d,e) f g h))
tunel m (Ghost (GhoState (a,(x,0),b,c,d,e) f)) = (Ghost (GhoState (a,(x,y-1),b,L,d,e) f))
                                                    where y = length (head m) 
tunel m (Ghost (GhoState (a,(x,y),b,c,d,e) f)) = (Ghost (GhoState (a,(x,0),b,R,d,e) f))                            


-- | Move the Player that corresponds to the ID, depending on the Orientation and on the Piece in the next position
playWith :: Maze -> Int -> Orientation -> [Player] -> [Player]
playWith m id ori [] = [] 
playWith m id ori (p1@(Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) | encontraID id p1 = movWhenEat m id ori (whosPiece m ori p1 t) (p1:t)
                                                                     | otherwise        = playWith m id ori (t++[p1]) 
playWith m id ori (p1@(Ghost (GhoState (a,(x,y),b,c,d,e) f)):t) | encontraID id p1 = movWhenEat m id ori (whosPiece m ori p1 t) (p1:t)
                                                                | otherwise        = playWith m id ori (t++[p1])                        



-- | Change the list of Players in the State, depending on the ID, on the Orientation and on the Maze 
play :: Play -> State -> State 
play (Move id ori) (State m lj lv) = (State m (playWith m id ori lj) lv) 



