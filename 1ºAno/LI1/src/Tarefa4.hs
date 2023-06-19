{- |
 - O objetivo da Tarefa 4 é reagir à passagem do tempo e para isso tivemos de definir a função passTime

 - Nesta tarefa, fizemos várias funções auxiliares. Primeiro, definimos uma função chasePac que faz alterar o estado da boca do Pacman, depois definimos outra função lostTime para o Pacman enquanto em Mega, perder tempo Mega. A função changeModeGhost faz alterar o State de entrada se não encontrar modo Mega em nenhum Pacman.
   Para a função encontraMega, se o Pacman presente numa lista de Players tiver em Mega, então a função irá dar False, por sua vez se não estiver em Mega irá dar True.
   A função changeModePac transforma o tempo Mega em zero e muda o Modo dos Pacmans presentes no State para Normal.
   Para a função encontraTempo, se numa lista de Players, o tempo Mega de um Pacman for menor ou igual a zero então dá True, e por sua vez se não encontar irá dar False.
   A função convertPac, transforma um State numa lista de jogadores.
   A encontraPac vai procurar o Pacman numa lista de jogadores.
   A função removePiece é usada para efeitos visuais no próprio jogo, pois quando o Pacman passa por cima das Food Big e Food Little, estas desaparecem.
   A função newPlay, vai ser uma nova função play com as novas caracteristicas.
   A função passTime, faz determinar as ações em cada instante por parte dos Players.
   A função pacTop coloca o PacMan no final da lista.

 - Em suma, o principal resultado obtido com esta tarefa foi a implementação da passagem do tempo nas jogadas -}




module Tarefa4 where 

import Types
import Tarefa2
import Tarefa5



defaultDelayTime = 250 -- 250 ms

-- | Faz mudar o estado da boca do Player Pacman 
--
changeMouth :: State -> State
changeMouth (State m ((Pacman ((PacState (a,(x,y),b,c,d,e) f g h))):t) l) | g==Open = (State m ((Pacman ((PacState (a,(x,y),b,c,d,e) f Closed h))):t) l)
                                                                          | otherwise = (State m ((Pacman ((PacState (a,(x,y),b,c,d,e) f Open h))):t) l)
changeMouth (State m (x:t) l) = changeMouth (State m (t++[x]) l)

-- | Faz perder tempo Mega enquanto está em Mega
--
lostTime :: [Player] -> [Player]
lostTime [] = [] 
lostTime ((Pacman (PacState (a,(x,y),b,c,d,e) f g Mega)):t) = ((Pacman (PacState (a,(x,y),b,c,d,e) (f-0.25) g Mega)):t)
lostTime (x:t) = x : lostTime t

-- | Se não encontra modo Mega, então o State vai se alterar
--
changeModeGhost :: State -> State
changeModeGhost w@(State m lj l) | encontraMega lj = (State m (auxchangeModeGhost lj) l)
                                 | otherwise = w

-- | Dada uma lista de Players, muda a velocidade dos fantasmas para 1 e o Modo para Alive
--
auxchangeModeGhost :: [Player] -> [Player]
auxchangeModeGhost [] = []
auxchangeModeGhost ((Ghost (GhoState (a,(x,y),b,c,d,e) f)):t) = (Ghost (GhoState (a,(x,y),1,c,d,e) Alive)) : auxchangeModeGhost t 
auxchangeModeGhost ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) = (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) : auxchangeModeGhost t 

-- | Se não encontrar nenhum Pacman com modo Mega dá True, se encontrar da False
encontraMega :: [Player] -> Bool
encontraMega [] = True
encontraMega ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)) : xs) | h==Mega = False
                                                                | otherwise = True
encontraMega (_:t) = encontraMega t

-- | Muda o modo do Pacman para Normal
changeModePac :: State -> State
changeModePac w@(State m lj l) | encontraTempo lj = (State m (auxchangeModePac lj) l)
                               | otherwise = w

-- | Muda o tempo dos Pacmans na lista de Players para 0 e o seu modo para Normal
auxchangeModePac :: [Player] -> [Player]
auxchangeModePac [] = [] 
auxchangeModePac ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) = (Pacman (PacState (a,(x,y),b,c,d,e) 0 g Normal)) : auxchangeModePac t 
auxchangeModePac (h:t) = h : auxchangeModeGhost t  

-- | Se encontrar tempo Mega menor ou igual a 0 dá True, se não dá False
encontraTempo :: [Player] -> Bool
encontraTempo ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) | f<=0 = True
                                                              | otherwise = False
encontraTempo ((_:t)) = encontraTempo t

-- | Transforma o State na lista de jogadores que se encontra no State
convertPac :: State -> [Player]
convertPac (State m lj lv) = lj 

-- | Se encontra o Pacman retorna esse Pacman
encontraPac :: [Player] -> Player
encontraPac (w@(Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) = w
encontraPac (_:t) = encontraPac t 

-- | Remove as pieces quando o Pacman passa por elas
removePiece :: State -> State 
removePiece (State m lj lv) = (State (auxRemove m (encontraPac lj)) lj lv) 

-- | Determina a peça a remover e substitui por Empty
auxRemove :: Maze -> Player -> Maze
auxRemove m (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) = take (x) m ++ [(take (y) l1) ++ [Empty] ++ (drop (y+1) l1)] ++ drop (x+1) m 
                                                        where l1 = head (drop (x) m)

-- | Nova função play
newPlay :: Play -> State -> State 
newPlay (Move id ori) (State m lj lv) = changeModeGhost $ changeModePac $ removePiece $ (changeMouth (State m1 (lostTime lj1) lv1))
                                 where (State m1 lj1 lv1) = play (Move id ori) (State m lj lv)  

-- | Altera o estado do jogo em uma iteração
passTime :: Int -> State -> State
passTime 0 s = s 
passTime i (State s lj lv) = auxPass i (pacEnd lj) (State s (pacEnd lj) lv)
 
-- | Função auxiliar para a passTime
auxPass :: Int -> [Player] -> State -> State 
auxPass _ [] s = s
auxPass i (p1@(Ghost (GhoState (a,(x,y),0.5,c,d,e) f)):xs) s | even i = auxPlays (ghostPlay s) s
                                                             | otherwise = s 
auxPass i (p1@(Ghost (GhoState (a,(x,y),1.0,c,d,e) f)):xs) s = auxPlays (ghostPlay s) s 
auxPass i (p1@(Pacman (PacState (a,(x,y),v,c,d,e) f g h)):xs) s = (newPlay (Move a c) (auxPlays (ghostPlay s) s))

-- | Coloca o Pacman no final da lista de Players
pacTop :: [Player] -> [Player]
pacTop [] = [] 
pacTop ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):xs) = xs ++ [(Pacman (PacState (a,(x,y),b,c,d,e) f g h))] 
pacTop ((Ghost (GhoState (a,(x,y),b,c,d,e) f)):xs) = (Ghost (GhoState (a,(x,y),b,c,d,e) f)) : pacTop xs

-- | Aplica uma lista de Plays num estado
auxPlays :: [Play] -> State -> State
auxPlays [] s = s 
auxPlays (x:xs) s = auxPlays xs (newPlay x s)




