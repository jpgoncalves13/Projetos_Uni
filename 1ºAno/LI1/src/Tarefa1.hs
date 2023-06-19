{- | 
-   Nesta tarefa, o principal objetivo é gerar um labirinto válido de acordo com o jogo original.
    Para alcançar esse objetivo, tivemos de definir a função generateMaze que dado a altura e o comprimento do labirinto e uma seed aleatória, produz um labirinto completamente válido.

-   Ao ler o enunciado do projeto, percebemos que tinhamos de realizar várias tarefas, tal como colocar a casa dos fantasmas no meio do labirinto, com algumas restrições caso o comprimento do labirinto fosse par, ou ímpar,
    como também colocar os tuneis, tanto no canto direito, como no canto esquerdo dependendo se a altura é ímpar ou se a altura é par.
    Em relação à casa dos fantasmas, para começar fizemos uma função oddHouse que representa a casa dos fantasmas quando o labirinto tem comprimento ímpar, e fizemos uma função evenHouse que representa a casa dos fantasmas quando o labirinto tem comprimento par.
    Logo a seguir, tivemos de escolher a lista de corredores onde ia ser implementada a casa dos fantasmas, e foi para isso que fizemos a função chooseCorrHouse que dado um maze produz essa lista de corredores. Ainda implementámos a putLine que escolhe um corredor de uma lista de corredores dependendo do numero inteiro dado.
    Por fim, fizemos a função putHouse onde alteramos a lista de corredores (que representa o labirinto) para colocar a casa dos fantasmas, utilizando as funções evenHouse, oddHouse e putLine.
    Quanto à implementação do tunel, criamos uma função denominada por linesTunnel que escolhe a linha do meio do labirinto, uma função putEmpty que coloca peças Emptys no ínicio e no final dos corredores de uma lista de corredores, a changeMidCorr que dado um Maze escolhe uma lista de corredores onde ficará o tunel, e por fim 
    a generateTunnel que utilizando as funções auxiliares anteriormente definidas para o tunel, gera os tuneis no labirinto.
    Para gerar um labirinto válido, também tivemos de fazer duas funções auxiliares : a buildWalls que coloca paredes na parte de cima e na parte de baixo do labirinto, e a putWalls que coloca paredes na parte direita e na parte esquerda do labirinto
    Por fim, fizemos a generateMaze, que utilizando as funções buildWalls, generateTunnel, putHouse, changeMidCorr e também tendo em conta se a altura do labirinto é impar ou par, gera um labirinto válido.

-   Em suma, os principais resultados obtidos nesta Tarefa foram a construção de um labirinto válido e a implementação do tunel e da casa dos fantasmas.  -}

module Tarefa1 where 

import Types
import System.Random 

-- | Given a seed returns a list of n integer randomly generated
--
geraAleatorios :: Int -> Int -> [Int]
geraAleatorios n seed = let gen = mkStdGen seed -- creates a random generator
                        in take n $ randomRs (0,90) gen -- takes the first n elements from an infinite series of random numbers between 0-9


-- | Given a seed returns an integer randomly generated
--
nrAleatorio :: Int -> Int
nrAleatorio seed = head $ geraAleatorios 1 seed


-- | Converts a list into a list of list of size n
--
subLista :: Int -> [a] -> [[a]]
subLista _ [] = []
subLista n l = take n l: subLista n (drop n l)


-- | Converts an integer number into a Piece
--
convertePiece  :: Int -> Piece 
convertePiece  x | x == 3 = Food Big
                 | x >= 0 && x < 70 = Food Little
                 | x >= 70 && x <= 90 = Wall

-- | Converts a Corridor to a string
--
printCorridor :: Corridor -> String
printCorridor [] = "\n"
printCorridor (h:t) = show h ++ printCorridor t

-- | Converts a list of integers into a Corridor
--
converteCorridor :: [Int] -> Corridor
converteCorridor l = map convertePiece  l

-- | Converts a list of lists of integers into a Maze
--
converteMaze :: [[Int]] -> Maze
converteMaze l = map converteCorridor l



--                               [CASA DOS FANTASMAS]

-- | Maze house for odd length
oddHouse :: [Corridor]
oddHouse = [[Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty],
             [Empty,Wall,Wall,Wall,Empty,Empty,Empty,Wall,Wall,Wall,Empty],
             [Empty,Wall,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Wall,Empty],
             [Empty,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Empty],
             [Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty]
            ]

-- | Maze house for even length
evenHouse :: [Corridor]
evenHouse = [[Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty],
           [Empty,Wall,Wall,Wall,Empty,Empty,Wall,Wall,Wall,Empty],
           [Empty,Wall,Empty,Empty,Empty,Empty,Empty,Empty,Wall,Empty],
           [Empty,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Empty],
           [Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty]
          ]

-- | Choose a list of corridors for the ghost house
chooseCorrHouse :: Maze -> [Corridor]
chooseCorrHouse m | even (length m) = take 5 (drop (metade-3) m)  
                  | otherwise       = take 5 (drop (metade-2) m) 
                  where metade = linesTunnel m  

-- | Choose the right corridor
putLine :: Int -> [Corridor] -> Corridor
putLine n (x:xs) | n==1 = x 
                 | otherwise = putLine (n-1) xs 

-- | Put the ghost house in the middle of the maze
putHouse :: [Corridor] -> [Corridor] 
putHouse (x:[]) | even (length x) = [(take (metade-5) x ++ (putLine 5 evenHouse) ++ drop (metade+5) x)] 
                | otherwise       = [(take (metade-5) x ++ (putLine 5 oddHouse) ++ drop (metade+6) x)]              
          where metade = div (length x) 2 
putHouse (x:xs) | even (length x) = let c = 5 - length xs in ((take (metade-5) x ++ (putLine c evenHouse) ++ drop (metade+5) x) : putHouse xs) 
                | otherwise       = let c = 5 - length xs in ((take (metade-5) x ++ (putLine c oddHouse) ++ drop (metade+6) x) : putHouse xs)                      
          where metade = div (length x) 2 

--                                      [TUNEL]

-- | Choose the middle line of the tunnel depending on the height of the maze
linesTunnel :: Maze -> Int 
linesTunnel m = div (length m) 2 
                 
-- | Generate Corridors with emptys in the beginning and in the end of the maze
putEmpty :: [Corridor] -> [Corridor] 
putEmpty [x]    = [[Empty] ++ (tail (init x)) ++ [Empty]] 
putEmpty (x:xs) = ([Empty] ++ (tail (init x)) ++ [Empty]) : putEmpty xs 

-- | Choose the right Corridors for the tunnel
changeMidCorr :: Maze -> [Corridor]
changeMidCorr m | even (length m) = take 2 (drop (n-1) m)  
                | otherwise       = take 1 (drop n m) 
              where n = linesTunnel m 

-- | Creates the tunnel in the maze
generateTunnel :: Maze -> Maze 
generateTunnel m | odd (length m) = putWalls (take metade m) ++ putEmpty (changeMidCorr m) ++ putWalls (drop (metade+1) m) 
                 | otherwise      = putWalls (take (metade-1) m) ++ putEmpty (changeMidCorr m) ++ putWalls (drop (metade+1) m) 
                 where metade = linesTunnel m 


--                              [LABIRINTO VÁLIDO]

-- | Creates a corridor only with Wall pieces, whereas the integer is non-negative
buildWalls :: Int -> Corridor 
buildWalls x | x == 0    = [] 
             | otherwise = Wall : buildWalls (x-1)   

-- | Put walls on the edge of the list of Corridors 
putWalls :: [Corridor] -> [Corridor]
putWalls [] = [] 
putWalls [x] = [[Wall] ++ tail (init x) ++ [Wall]] 
putWalls (x:xs) = ([Wall] ++ (tail (init x)) ++ [Wall]) : putWalls xs 

-- | Creates a valid maze
generateMaze :: Int -> Int -> Int -> Maze
generateMaze x y s | even y =  let m = [buildWalls x] ++ generateMaze1 ++ [buildWalls x] in generateTunnel (take (n-3) m ++ putHouse (chooseCorrHouse m) ++ drop (n+2) m) 
                   | otherwise = let m = [buildWalls x] ++ generateMaze1 ++ [buildWalls x] in generateTunnel (take (n-2) m ++ putHouse (chooseCorrHouse m) ++ drop (n+3) m) 
            where n = div y 2 
                  generateMaze1 = let random_nrs = geraAleatorios (x*(y-2)) s in converteMaze $ subLista x random_nrs 
