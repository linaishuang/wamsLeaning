      SUBROUTINE calldgelss(m,n,A,B,X)

      INTEGER m,n,lda,ldb,INFO
      DOUBLE PRECISION A(m*n,1),B(m,1),X(n,1)
      DOUBLE PRECISION,ALLOCATABLE :: A1(:,:),A2(:,:)
      INTEGER i,j,LWORK,RANK
      DOUBLE PRECISION,ALLOCATABLE :: S(:), WORK(:)      

C      ALLOCATE(A(m*n))
      lda = max(m,n)
      ldb = max(m,1)
      nrhs = 1
      LWORK=3*min(m,n)+max(2*min(m,n),max(m,n),1)
      ALLOCATE(A1(n,m))
      ALLOCATE(A2(m,n))
      ALLOCATE(S(lda))
      ALLOCATE(WORK(LWORK))

      A1 = 0.0D0
      S = 0.0D0
      WORK = 0.0D0

      A1 = reshape(A,(/n,m/))            
      A2 = transpose(A1)
      CALL dgelss(m,n,1,A2,lda,B,ldb,S,0.0000001D0, RANK, WORK, LWORK,
     &            INFO)            
      IF(INFO .EQ. 0) THEN
        X(1:n,1) = B(1:n,1)
      ELSE
        PRINT *,'LAPACK：方程无解！'
      ENDIF

      deallocate(A1)
      deallocate(A2)
      deallocate(S)
      deallocate(WORK)

      END SUBROUTINE calldgelss