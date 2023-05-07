# i n c l u d e   " C h e s s P i e c e s . h "  
 # i n c l u d e   < Q M o u s e E v e n t >  
 # i n c l u d e   < Q P i x m a p >  
 # i n c l u d e   < Q P a l e t t e >  
 # i n c l u d e   < Q D e b u g >  
  
 C h e s s P i e c e s : : C h e s s P i e c e s ( c o n s t   P o s i t i o n &   p o s ,   T Y P E   t y p e ,   C O L O R   c o l o r ,   Q W i d g e t *   p a r e n t )  
         :   Q L a b e l ( p a r e n t ) ,   p o s ( p o s ) ,   t y p e ( t y p e ) ,   c o l o r ( c o l o r )  
 {  
  
 }  
  
 C h e s s P i e c e s : : C h e s s P i e c e s ( c o n s t   P o s i t i o n &   p o s ,   T Y P E   t y p e ,   C O L O R   c o l o r )  
         :   p o s ( p o s ) ,   t y p e ( t y p e ) ,   c o l o r ( c o l o r )  
 {  
  
 }  
  
 C h e s s P i e c e s : : C h e s s P i e c e s ( i n t   r o w ,   i n t   c o l ,   Q W i d g e t *   p a r e n t )   :   Q L a b e l ( p a r e n t ) {  
         p o s   =   P o s i t i o n {   r o w ,   c o l   } ;  
         i n t   i d   =   r o w   *   8   +   c o l ;  
  
         Q S t r i n g   p A d d r e s s ;  
  
         t h i s - > s e t F i x e d S i z e ( 8 0 ,   8 9 ) ;  
  
  
         s w i t c h   ( i d )   {  
         c a s e   0 :   c a s e   7 :  
                 t y p e     =   T Y P E : : R O O K ;  
                 c o l o r   =   C O L O R : : B L A C K ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / b _ r o o k _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   1 :   c a s e   6 :  
                 t y p e     =   T Y P E : : K N I G H T ;  
                 c o l o r   =   C O L O R : : B L A C K ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / b _ k n i g h t _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   2 :   c a s e   5 :  
                 t y p e     =   T Y P E : : B I S H O P ;  
                 c o l o r   =   C O L O R : : B L A C K ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / b _ b i s h o p _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   3 :  
                 t y p e     =   T Y P E : : Q U E E N ;  
                 c o l o r   =   C O L O R : : B L A C K ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / b _ q u e e n _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   4 :  
                 t y p e     =   T Y P E : : K I N G ;  
                 c o l o r   =   C O L O R : : B L A C K ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / b _ k i n g _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
  
         c a s e   8 :   c a s e   9 :   c a s e   1 0 :   c a s e   1 1 :   c a s e   1 2 :   c a s e   1 3 :   c a s e   1 4 :   c a s e   1 5 :  
                 t y p e     =   T Y P E : : P A W N ;  
                 c o l o r   =   C O L O R : : B L A C K ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / b _ p a w n _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   5 6 :   c a s e   6 3 :  
                 t y p e     =   T Y P E : : R O O K ;  
                 c o l o r   =   C O L O R : : W H I T E ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / w _ r o o k _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   5 7 :   c a s e   6 2 :  
                 t y p e     =   T Y P E : : K N I G H T ;  
                 c o l o r   =   C O L O R : : W H I T E ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / w _ k n i g h t _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   5 8 :   c a s e   6 1 :  
                 t y p e     =   T Y P E : : B I S H O P ;  
                 c o l o r   =   C O L O R : : W H I T E ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / w _ b i s h o p _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   5 9 :  
                 t y p e     =   T Y P E : : Q U E E N ;  
                 c o l o r   =   C O L O R : : W H I T E ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / w _ q u e e n _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   6 0 :  
                 t y p e     =   T Y P E : : K I N G ;  
                 c o l o r   =   C O L O R : : W H I T E ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / w _ k i n g _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         c a s e   4 8 :   c a s e   4 9 :   c a s e   5 0 :   c a s e   5 1 :   c a s e   5 2 :   c a s e   5 3 :   c a s e   5 4 :   c a s e   5 5 :  
                 t y p e     =   T Y P E : : P A W N ;  
                 c o l o r   =   C O L O R : : W H I T E ;  
                 p A d d r e s s   =   " : / p i e c e s / c h e s s P i e c e s / w _ p a w n _ p n g _ 1 2 8 p x . p n g " ;  
                 b r e a k ;  
         d e f a u l t :  
                 t y p e   =   T Y P E : : E M P T Y ;  
                 c o l o r   =   C O L O R : : W H I T E ;  
                 b r e a k ;  
         }  
  
         Q P i x m a p   p i e c e s I m a g e ( p A d d r e s s ) ;  
         t h i s - > s e t P i x m a p ( p i e c e s I m a g e ) ;  
  
 }  
  
 / /   a 1   - >   P o s i t i o n { 7 ,   0 }  
 P o s i t i o n   p o s F r o m A l g e b r a i c ( s t d : : s t r i n g   n o t a t i o n )  
 {  
         P o s i t i o n   p o s ;  
         p o s . r o w   =   8   -   ( n o t a t i o n . a t ( 1 )   -   ' 0 ' ) ;  
         p o s . c o l   =   n o t a t i o n . a t ( 0 )   -   ' a ' ;  
         r e t u r n   p o s ;  
 }  
  
 b o o l   i s O n B o a r d ( c o n s t   P o s i t i o n   & p o s )  
 {  
         r e t u r n   0   < =   p o s . r o w   & &   p o s . r o w   <   8   & &   0   < =   p o s . c o l   & &   p o s . c o l   <   8 ;  
 }  
  
 v o i d   C h e s s P i e c e s : : m o u s e P r e s s E v e n t ( Q M o u s e E v e n t *   e v e n t )  
 {  
         i f   ( e v e n t - > b u t t o n ( )   = =   Q t : : L e f t B u t t o n )   {  
                 q D e b u g ( )   < <   " c l i c k "   < <   " \ n " ;  
                 e m i t   c l i c k e d ( p o s ) ;  
  
         }  
 }  
 