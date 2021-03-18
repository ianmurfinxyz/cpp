

//
// An old question now found...
//

void skinCharacter(vert *out, vert* in, u32 numVerts)
{
  vert *input = allocateFromCache(HALF_CACHE_SIZE);
  vert *output = allocateFromCache(HALF_CACHE_SIZE);
  while(numVerts){
    u32 = MIN(numVerts, HALF_CACHE_SIZE/sizeof(vert));
    TransferHandle h = streamIntoCache(input, in, n*sizeof(vert));
    streamWaitForTransfer(h);
    for(int i {0}; i < n; ++i){
      skinVertex(&output[i], &input[i]); 
    }
    h = streamOutOfCache(out, output, n * sizeof(vert));
    streamWaitForTransfer(h);
    numVerts -= n;
  }
}

//
// this is a simple skinning function that skins vertices on the CPU. It uses DMA to transfer
// the data into and out of the cache to speed up the processing that goes on in the skinVertex
// function. The waits are wasted time though, how would you rewrite this function to minimise
// this?
//
