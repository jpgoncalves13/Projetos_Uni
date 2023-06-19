{- | 
  - O objetivo desta Tarefa foi de implementar um bot que jogue como Player Pacman por nós. 

  - Primeiramente, começamos por fazer algumas funções auxiliares, tal como a idPlayer que dado um Id e uma lista de Players, escolhe um Player, a função removePac, que dado um id, e uma lista de Players, remove o Player Pacman que tem um id igual ao dado, 
    a função compareCoords que produz uma lista com as distâncias de Manhattan do Pacman para os restantes Ghosts, a funcao whatPlayer que escolhe um jogador de uma lista de Players dependendo do inteiro dado e a função whatPlay que dados dois Players, um Pacman e um Ghost e comparando as coordenadas dos dois, faz a melhor jogada possível para o Pacman.
    Por fim, a funcao auxBotPlay produz uma possível jogada para o bot e vai ser implementada na função bot.
    
  - Concluindo, os resultados mais importantes dados por estas Tarefas foram  a implementação de jogadas automáticas no jogo    -}


module Tarefa6 where 

import Types

-- | Escolhe um Player Pacman de uma lista de Players, baseado no seu ID
--
idPlayer :: Int -> [Player] -> Player 
idPlayer id (p1@(Pacman (PacState (a,(x,y),b,c,d,e) f g h)):ys) | id == a = p1
                                                                | otherwise = idPlayer id ys  
idPlayer id (p1@(Ghost (GhoState (a,(x,y),b,c,d,e) f)):ys) = idPlayer id ys 

-- | Remove o Pacman de uma lista de Players
--
removePac :: Int -> [Player] -> [Player]
removePac id (p1@(Pacman (PacState (a,(x,y),b,c,d,e) f g h)):ys) | id == a = ys
                                                                 | otherwise = p1 : removePac id ys 
removePac id (p1@(Ghost (GhoState (a,(x,y),b,c,d,e) f)):ys) = p1 : removePac id ys 

-- | Lista das distâncias de Manhattan do Pacman a cada Ghost
--
compareCoords :: Player -> [Player] -> [Int] 
compareCoords p1@(Pacman (PacState (a1,(x1,y1),b1,c1,d1,e1) f1 g1 h1)) ((Ghost (GhoState (a,(x,y),b,c,d,e) Alive)):t) = (abs (x1-x) + abs (y1-y)) : compareCoords p1 t  
compareCoords p1@(Pacman (PacState (a1,(x1,y1),b1,c1,d1,e1) f1 g1 h1)) ((Ghost (GhoState (a,(x,y),b,c,d,e) Dead)):t)  = (abs (x1-x) + abs (y1-y)) : compareCoords p1 t

-- | Escolhe um Player de uma lista de Players dependendo no Inteiro dado
--                  
whatPlayer :: Int -> [Player] -> Player
whatPlayer a (y:ys) | a == 0 = y 
                    | otherwise = whatPlayer (a-1) ys 

-- | Realiza as jogadas para o Pacman 
--
whatPlay :: Player -> Player -> Maybe Play
whatPlay p1@(Pacman (PacState (a1,(x1,y1),b1,c1,d1,e1) f1 g1 h1)) p2@(Ghost (GhoState (a,(x,y),b,c,d,e) Alive)) | x1 == x && y1 >= y = Just (Move a1 R)
                                                                                                                | x1 == x && y1 <= y = Just (Move a1 L)
                                                                                                                | x1 <= x && y1 == y = Just (Move a1 U) 
                                                                                                                | x1 >= x && y1 == y = Just (Move a1 D) 
                                                                                                                | y1 <= y && x1 <= x = Just (Move a1 L)
                                                                                                                | y1 >= y && x1 >= x = Just (Move a1 R) 
                                                                                                                | y1 >= y && x1 <= x = Just (Move a1 R)
                                                                                                                | y1 <= y && x1 >= x = Just (Move a1 L)
whatPlay p1@(Pacman (PacState (a1,(x1,y1),b1,c1,d1,e1) f1 g1 h1)) p2@(Ghost (GhoState (a,(x,y),b,c,d,e) Dead)) | x1 == x && y1 >= y = Just (Move a1 L)
                                                                                                               | x1 == x && y1 <= y = Just (Move a1 R)
                                                                                                               | x1 <= x && y1 == y = Just (Move a1 D) 
                                                                                                               | x1 >= x && y1 == y = Just (Move a1 U) 
                                                                                                               | y1 <= y && x1 <= x = Just (Move a1 R)
                                                                                                               | y1 >= y && x1 >= x = Just (Move a1 L) 
                                                                                                               | y1 >= y && x1 <= x = Just (Move a1 L)
                                                                                                               | y1 <= y && x1 >= x = Just (Move a1 R)

-- | Função auxiliar para a função bot  
--
auxBotPlay :: Player -> [Player] -> Maybe Play 
auxBotPlay x (y:ys) = whatPlay x (whatPlayer (minimum (compareCoords x (y:ys))) (y:ys)) 

-- | Possível jogada a realizar pelo bot
bot :: Int -> State -> Maybe Play
bot x (State m (y:ys) l) = auxBotPlay (idPlayer x (y:ys)) (removePac x (y:ys)) 

