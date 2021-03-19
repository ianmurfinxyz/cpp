

//
// An old question now found...
//

void skinCharacter(vert *out, vert* in, u32 numVerts)
{
  vert *input = allocateFromCache(HALF_CACHE_SIZE);
  vert *output = allocateFromCache(HALF_CACHE_SIZE);
  while(numVerts){
    u32 n = MIN(numVerts, HALF_CACHE_SIZE/sizeof(vert));
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

//
// ANSWER 1 - the question states "rewrite this function to minimise", so assuming that this has
// to be a single function...
//
// note that the initial version allocates 2 seperate buffers on the CPU cache, an ouput and
// input, therefore we should be able to stream data in/out of them at the same time, so we could
// instead do...
//

void skinCharacter(vert *out, vert* in, u32 numVerts)
{
  vert *input = allocateFromCache(HALF_CACHE_SIZE);
  vert *output = allocateFromCache(HALF_CACHE_SIZE);
  u32 n = MIN(numVerts, HALF_CACHE_SIZE/sizeof(vert));
  TransferHandle ih, oh;
  ih = streamIntoCache(input, in, n*sizeof(vert));
  streamWaitForTransfer(ih);
  while(numVerts){
    for(int i {0}; i < n; ++i)
      skinVertex(&output[i], &input[i]); 

    //
    // assuming the DMA chip can handle multiple channels, we could minimise the waits by
    // streaming the next input into the cache at the same time we stream the last input out
    // of cache.
    //
    oh = streamOutOfCache(out, output, n * sizeof(vert));
    numVerts -= n;
    n = MIN(numVerts, HALF_CACHE_SIZE/sizeof(vert));
    ih = streamIntoCache(input, in, n*sizeof(vert));
    streamWaitForTransfer(ih);
    streamWaitForTransfer(oh);
  }
}

//
// This would minimise the waits but it would not remove all wasted time.
//

//
// Answer 2 - callbacks
//
// Assuming we had other work to do we could split the function up into callbacks that run 
// when the transfers are complete, so we can use the wait time to do something else.
//

// TODO: write this answer!

void someFunction()
{
  while(worktoDo){
    
    // do some work...
  }
}



