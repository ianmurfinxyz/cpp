
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// The question:
// 
// void recurseDecodeSecretMessage(node *n)
// {
//   char temp[MAX_MESSAGE_LENGTH];
//   decodeSecreteMessage(temp, n->message, n->key);
//   if(strcmp(temp, "Go Left") == 0)
//     recurseDecodeSecretMessage(n->leftNode);
//   else
//     recurseDecodeSecretMessage(n->rightNode);
// }
//
// This is arecursive function that transverses a tree by decoding secret messages. What 
// problem would you run into if the tree was very deep (apart from it being slow)?
//
// Can you rearrange it to solve this problem?
//
//////////////////////////////////////////////////////////////////////////////////////////////////

//
// The answer is you would encounter a stack overflow due to allocating the temp buffer in
// every 'recurseDcodeSecretMessage' call's stack frame.
//
// The solution would be to simpy move the allocation of the buffer into the decodeSecretMessage
// function and have said function return some value (say a bool or int) to indicate the
// direction to take. This would ensure the temp buffer is popped after each call to the decode
// function so the recurse function has a much smaller stack frame.
//

//
// The following is an example that shows this to in fact be the problem and solution by 
// monitoring the address of the stack for each recursive call with and without the buffer.
//
// note: could also count the number of recursions it takes to cause an overflow with and 
// without said buffer. You know crash my computer for science!
//
//
// remember that the stack grows toward lower address, i.e. starts at a high address.

#include <iostream>

void foo_with_inline_buffer(int depth, int first = 1, char* stack_of_first = nullptr, char* stack_of_prev = nullptr)
{
  char c[1024];

  for(int i = 0; i < 1024; ++i) 
    c[i] = i;
  long long sum{0};
  for(int i = 0; i < 1024; ++i){
    sum += c[i];
  }
  std::cout << "sum=" << sum << std::endl;

  if(first > 0){
    stack_of_first = c;
    first = 0;
  }

  if(!stack_of_prev)
    stack_of_prev = c;

  std::cout << "stack address:" << static_cast<void*>(&c) 
            << " : as_long:" << static_cast<long>(c - stack_of_first)
            << " : from_prev:" << static_cast<long>(c - stack_of_prev)
            << " : [c=" << c << "]" << std::endl; // use c too so not stripped by compiler

  --depth;
  if(depth < 0)
    return;
  else
    foo_with_inline_buffer(depth, first, stack_of_first, c);
}

long long allocate_and_pop_a_buffer()
{
  int buffer[1024]; 
  for(int i = 0; i < 1024; ++i) 
    buffer[i] = i;
  long long sum{0};
  for(int i = 0; i < 1024; ++i){
    sum += buffer[i];
  }
  return sum;
}

void foo_with_call_buffer(int depth, int first = 1, char* stack_of_first = nullptr, char* stack_of_prev = nullptr)
{
  char c = 'a';

  if(first > 0){
    stack_of_first = &c;
    first = 0;
  }

  //
  // make sure this is actually needed so GCC doesn't decide it is not.
  //
  long long sum = allocate_and_pop_a_buffer();
  std::cout << "sum=" << sum << std::endl;

  if(!stack_of_prev)
    stack_of_prev = &c;

  std::cout << "stack address:" << static_cast<void*>(&c) 
            << " : as_long:" << static_cast<long>(&c - stack_of_first)
            << " : from_prev:" << static_cast<long>(&c - stack_of_prev)
            << " : [c=" << c << "]" << std::endl; // use c too so not stripped by compiler

  --depth;
  if(depth < 0)
    return;
  else
    foo_with_call_buffer(depth, first, stack_of_first, &c);
}

void foo(int depth, int first = 1, char* stack_of_first = nullptr, char* stack_of_prev = nullptr)
{
  char c = 'a';

  if(first > 0){
    stack_of_first = &c;
    first = 0;
  }

  if(!stack_of_prev)
    stack_of_prev = &c;

  std::cout << "stack address:" << static_cast<void*>(&c) 
            << " : as_long:" << static_cast<long>(&c - stack_of_first)
            << " : from_prev:" << static_cast<long>(&c - stack_of_prev)
            << " : [c=" << c << "]" << std::endl; // use c too so not stripped by compiler

  --depth;
  if(depth < 0)
    return;
  else
    foo(depth, first, stack_of_first, &c);
}

int main()
{
  std::cout << "================ foo with inline buffer =============" << std::endl;
  foo_with_inline_buffer(100);
  std::cout << "================ foo with call buffer =============" << std::endl;
  foo_with_call_buffer(100);
  std::cout << "================ foo =============" << std::endl;
  foo(100);
}



/*

   results below.

   clearly the foo with the inline buffer has a larger stack frame, as shown by from_prev=-1104,
   which means that each call to foo_with_inline_buffer is assing 1104 bytes to the call stack.

   The other calls only add 80 and 64 bytes for foo_with_call_buffer and foo respectively.

   thus QED. stack overflow could occur by allocating large buffers in recursive functions.
   Instead allocate them in a function called from the recursive function so the called 
   function, and its buffer, is popped off the stack prior to the next recursive call.

================ foo with inline buffer =============
sum=-512
stack address:0x7fffb560aca0 : as_long:0 : from_prev:0 : [c=]
sum=-512
stack address:0x7fffb560a850 : as_long:-1104 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb560a400 : as_long:-2208 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5609fb0 : as_long:-3312 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5609b60 : as_long:-4416 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5609710 : as_long:-5520 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56092c0 : as_long:-6624 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5608e70 : as_long:-7728 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5608a20 : as_long:-8832 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56085d0 : as_long:-9936 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5608180 : as_long:-11040 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5607d30 : as_long:-12144 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56078e0 : as_long:-13248 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5607490 : as_long:-14352 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5607040 : as_long:-15456 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5606bf0 : as_long:-16560 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56067a0 : as_long:-17664 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5606350 : as_long:-18768 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5605f00 : as_long:-19872 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5605ab0 : as_long:-20976 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5605660 : as_long:-22080 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5605210 : as_long:-23184 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5604dc0 : as_long:-24288 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5604970 : as_long:-25392 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5604520 : as_long:-26496 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56040d0 : as_long:-27600 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5603c80 : as_long:-28704 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5603830 : as_long:-29808 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56033e0 : as_long:-30912 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5602f90 : as_long:-32016 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5602b40 : as_long:-33120 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56026f0 : as_long:-34224 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56022a0 : as_long:-35328 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5601e50 : as_long:-36432 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5601a00 : as_long:-37536 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56015b0 : as_long:-38640 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5601160 : as_long:-39744 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5600d10 : as_long:-40848 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb56008c0 : as_long:-41952 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5600470 : as_long:-43056 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb5600020 : as_long:-44160 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55ffbd0 : as_long:-45264 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55ff780 : as_long:-46368 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55ff330 : as_long:-47472 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55feee0 : as_long:-48576 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fea90 : as_long:-49680 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fe640 : as_long:-50784 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fe1f0 : as_long:-51888 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fdda0 : as_long:-52992 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fd950 : as_long:-54096 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fd500 : as_long:-55200 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fd0b0 : as_long:-56304 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fcc60 : as_long:-57408 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fc810 : as_long:-58512 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fc3c0 : as_long:-59616 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fbf70 : as_long:-60720 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fbb20 : as_long:-61824 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fb6d0 : as_long:-62928 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fb280 : as_long:-64032 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fae30 : as_long:-65136 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fa9e0 : as_long:-66240 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fa590 : as_long:-67344 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55fa140 : as_long:-68448 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f9cf0 : as_long:-69552 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f98a0 : as_long:-70656 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f9450 : as_long:-71760 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f9000 : as_long:-72864 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f8bb0 : as_long:-73968 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f8760 : as_long:-75072 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f8310 : as_long:-76176 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f7ec0 : as_long:-77280 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f7a70 : as_long:-78384 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f7620 : as_long:-79488 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f71d0 : as_long:-80592 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f6d80 : as_long:-81696 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f6930 : as_long:-82800 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f64e0 : as_long:-83904 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f6090 : as_long:-85008 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f5c40 : as_long:-86112 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f57f0 : as_long:-87216 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f53a0 : as_long:-88320 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f4f50 : as_long:-89424 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f4b00 : as_long:-90528 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f46b0 : as_long:-91632 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f4260 : as_long:-92736 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f3e10 : as_long:-93840 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f39c0 : as_long:-94944 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f3570 : as_long:-96048 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f3120 : as_long:-97152 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f2cd0 : as_long:-98256 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f2880 : as_long:-99360 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f2430 : as_long:-100464 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f1fe0 : as_long:-101568 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f1b90 : as_long:-102672 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f1740 : as_long:-103776 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f12f0 : as_long:-104880 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f0ea0 : as_long:-105984 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f0a50 : as_long:-107088 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f0600 : as_long:-108192 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55f01b0 : as_long:-109296 : from_prev:-1104 : [c=]
sum=-512
stack address:0x7fffb55efd60 : as_long:-110400 : from_prev:-1104 : [c=]
================ foo with call buffer =============
sum=523776
stack address:0x7fffb560b09f : as_long:0 : from_prev:0 : [c=a]
sum=523776
stack address:0x7fffb560b04f : as_long:-80 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560afff : as_long:-160 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560afaf : as_long:-240 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560af5f : as_long:-320 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560af0f : as_long:-400 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560aebf : as_long:-480 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560ae6f : as_long:-560 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560ae1f : as_long:-640 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560adcf : as_long:-720 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560ad7f : as_long:-800 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560ad2f : as_long:-880 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560acdf : as_long:-960 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560ac8f : as_long:-1040 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560ac3f : as_long:-1120 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560abef : as_long:-1200 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560ab9f : as_long:-1280 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560ab4f : as_long:-1360 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560aaff : as_long:-1440 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560aaaf : as_long:-1520 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560aa5f : as_long:-1600 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560aa0f : as_long:-1680 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a9bf : as_long:-1760 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a96f : as_long:-1840 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a91f : as_long:-1920 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a8cf : as_long:-2000 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a87f : as_long:-2080 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a82f : as_long:-2160 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a7df : as_long:-2240 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a78f : as_long:-2320 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a73f : as_long:-2400 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a6ef : as_long:-2480 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a69f : as_long:-2560 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a64f : as_long:-2640 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a5ff : as_long:-2720 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a5af : as_long:-2800 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a55f : as_long:-2880 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a50f : as_long:-2960 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a4bf : as_long:-3040 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a46f : as_long:-3120 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a41f : as_long:-3200 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a3cf : as_long:-3280 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a37f : as_long:-3360 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a32f : as_long:-3440 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a2df : as_long:-3520 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a28f : as_long:-3600 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a23f : as_long:-3680 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a1ef : as_long:-3760 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a19f : as_long:-3840 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a14f : as_long:-3920 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a0ff : as_long:-4000 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a0af : as_long:-4080 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a05f : as_long:-4160 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560a00f : as_long:-4240 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609fbf : as_long:-4320 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609f6f : as_long:-4400 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609f1f : as_long:-4480 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609ecf : as_long:-4560 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609e7f : as_long:-4640 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609e2f : as_long:-4720 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609ddf : as_long:-4800 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609d8f : as_long:-4880 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609d3f : as_long:-4960 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609cef : as_long:-5040 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609c9f : as_long:-5120 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609c4f : as_long:-5200 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609bff : as_long:-5280 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609baf : as_long:-5360 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609b5f : as_long:-5440 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609b0f : as_long:-5520 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609abf : as_long:-5600 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609a6f : as_long:-5680 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb5609a1f : as_long:-5760 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56099cf : as_long:-5840 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560997f : as_long:-5920 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560992f : as_long:-6000 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56098df : as_long:-6080 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560988f : as_long:-6160 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560983f : as_long:-6240 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56097ef : as_long:-6320 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560979f : as_long:-6400 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560974f : as_long:-6480 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56096ff : as_long:-6560 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56096af : as_long:-6640 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560965f : as_long:-6720 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560960f : as_long:-6800 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56095bf : as_long:-6880 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560956f : as_long:-6960 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560951f : as_long:-7040 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56094cf : as_long:-7120 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560947f : as_long:-7200 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560942f : as_long:-7280 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56093df : as_long:-7360 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560938f : as_long:-7440 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560933f : as_long:-7520 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56092ef : as_long:-7600 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560929f : as_long:-7680 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560924f : as_long:-7760 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56091ff : as_long:-7840 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb56091af : as_long:-7920 : from_prev:-80 : [c=a]
sum=523776
stack address:0x7fffb560915f : as_long:-8000 : from_prev:-80 : [c=a]
================ foo =============
stack address:0x7fffb560b0a7 : as_long:0 : from_prev:0 : [c=a]
stack address:0x7fffb560b067 : as_long:-64 : from_prev:-64 : [c=a]
stack address:0x7fffb560b027 : as_long:-128 : from_prev:-64 : [c=a]
stack address:0x7fffb560afe7 : as_long:-192 : from_prev:-64 : [c=a]
stack address:0x7fffb560afa7 : as_long:-256 : from_prev:-64 : [c=a]
stack address:0x7fffb560af67 : as_long:-320 : from_prev:-64 : [c=a]
stack address:0x7fffb560af27 : as_long:-384 : from_prev:-64 : [c=a]
stack address:0x7fffb560aee7 : as_long:-448 : from_prev:-64 : [c=a]
stack address:0x7fffb560aea7 : as_long:-512 : from_prev:-64 : [c=a]
stack address:0x7fffb560ae67 : as_long:-576 : from_prev:-64 : [c=a]
stack address:0x7fffb560ae27 : as_long:-640 : from_prev:-64 : [c=a]
stack address:0x7fffb560ade7 : as_long:-704 : from_prev:-64 : [c=a]
stack address:0x7fffb560ada7 : as_long:-768 : from_prev:-64 : [c=a]
stack address:0x7fffb560ad67 : as_long:-832 : from_prev:-64 : [c=a]
stack address:0x7fffb560ad27 : as_long:-896 : from_prev:-64 : [c=a]
stack address:0x7fffb560ace7 : as_long:-960 : from_prev:-64 : [c=a]
stack address:0x7fffb560aca7 : as_long:-1024 : from_prev:-64 : [c=a]
stack address:0x7fffb560ac67 : as_long:-1088 : from_prev:-64 : [c=a]
stack address:0x7fffb560ac27 : as_long:-1152 : from_prev:-64 : [c=a]
stack address:0x7fffb560abe7 : as_long:-1216 : from_prev:-64 : [c=a]
stack address:0x7fffb560aba7 : as_long:-1280 : from_prev:-64 : [c=a]
stack address:0x7fffb560ab67 : as_long:-1344 : from_prev:-64 : [c=a]
stack address:0x7fffb560ab27 : as_long:-1408 : from_prev:-64 : [c=a]
stack address:0x7fffb560aae7 : as_long:-1472 : from_prev:-64 : [c=a]
stack address:0x7fffb560aaa7 : as_long:-1536 : from_prev:-64 : [c=a]
stack address:0x7fffb560aa67 : as_long:-1600 : from_prev:-64 : [c=a]
stack address:0x7fffb560aa27 : as_long:-1664 : from_prev:-64 : [c=a]
stack address:0x7fffb560a9e7 : as_long:-1728 : from_prev:-64 : [c=a]
stack address:0x7fffb560a9a7 : as_long:-1792 : from_prev:-64 : [c=a]
stack address:0x7fffb560a967 : as_long:-1856 : from_prev:-64 : [c=a]
stack address:0x7fffb560a927 : as_long:-1920 : from_prev:-64 : [c=a]
stack address:0x7fffb560a8e7 : as_long:-1984 : from_prev:-64 : [c=a]
stack address:0x7fffb560a8a7 : as_long:-2048 : from_prev:-64 : [c=a]
stack address:0x7fffb560a867 : as_long:-2112 : from_prev:-64 : [c=a]
stack address:0x7fffb560a827 : as_long:-2176 : from_prev:-64 : [c=a]
stack address:0x7fffb560a7e7 : as_long:-2240 : from_prev:-64 : [c=a]
stack address:0x7fffb560a7a7 : as_long:-2304 : from_prev:-64 : [c=a]
stack address:0x7fffb560a767 : as_long:-2368 : from_prev:-64 : [c=a]
stack address:0x7fffb560a727 : as_long:-2432 : from_prev:-64 : [c=a]
stack address:0x7fffb560a6e7 : as_long:-2496 : from_prev:-64 : [c=a]
stack address:0x7fffb560a6a7 : as_long:-2560 : from_prev:-64 : [c=a]
stack address:0x7fffb560a667 : as_long:-2624 : from_prev:-64 : [c=a]
stack address:0x7fffb560a627 : as_long:-2688 : from_prev:-64 : [c=a]
stack address:0x7fffb560a5e7 : as_long:-2752 : from_prev:-64 : [c=a]
stack address:0x7fffb560a5a7 : as_long:-2816 : from_prev:-64 : [c=a]
stack address:0x7fffb560a567 : as_long:-2880 : from_prev:-64 : [c=a]
stack address:0x7fffb560a527 : as_long:-2944 : from_prev:-64 : [c=a]
stack address:0x7fffb560a4e7 : as_long:-3008 : from_prev:-64 : [c=a]
stack address:0x7fffb560a4a7 : as_long:-3072 : from_prev:-64 : [c=a]
stack address:0x7fffb560a467 : as_long:-3136 : from_prev:-64 : [c=a]
stack address:0x7fffb560a427 : as_long:-3200 : from_prev:-64 : [c=a]
stack address:0x7fffb560a3e7 : as_long:-3264 : from_prev:-64 : [c=a]
stack address:0x7fffb560a3a7 : as_long:-3328 : from_prev:-64 : [c=a]
stack address:0x7fffb560a367 : as_long:-3392 : from_prev:-64 : [c=a]
stack address:0x7fffb560a327 : as_long:-3456 : from_prev:-64 : [c=a]
stack address:0x7fffb560a2e7 : as_long:-3520 : from_prev:-64 : [c=a]
stack address:0x7fffb560a2a7 : as_long:-3584 : from_prev:-64 : [c=a]
stack address:0x7fffb560a267 : as_long:-3648 : from_prev:-64 : [c=a]
stack address:0x7fffb560a227 : as_long:-3712 : from_prev:-64 : [c=a]
stack address:0x7fffb560a1e7 : as_long:-3776 : from_prev:-64 : [c=a]
stack address:0x7fffb560a1a7 : as_long:-3840 : from_prev:-64 : [c=a]
stack address:0x7fffb560a167 : as_long:-3904 : from_prev:-64 : [c=a]
stack address:0x7fffb560a127 : as_long:-3968 : from_prev:-64 : [c=a]
stack address:0x7fffb560a0e7 : as_long:-4032 : from_prev:-64 : [c=a]
stack address:0x7fffb560a0a7 : as_long:-4096 : from_prev:-64 : [c=a]
stack address:0x7fffb560a067 : as_long:-4160 : from_prev:-64 : [c=a]
stack address:0x7fffb560a027 : as_long:-4224 : from_prev:-64 : [c=a]
stack address:0x7fffb5609fe7 : as_long:-4288 : from_prev:-64 : [c=a]
stack address:0x7fffb5609fa7 : as_long:-4352 : from_prev:-64 : [c=a]
stack address:0x7fffb5609f67 : as_long:-4416 : from_prev:-64 : [c=a]
stack address:0x7fffb5609f27 : as_long:-4480 : from_prev:-64 : [c=a]
stack address:0x7fffb5609ee7 : as_long:-4544 : from_prev:-64 : [c=a]
stack address:0x7fffb5609ea7 : as_long:-4608 : from_prev:-64 : [c=a]
stack address:0x7fffb5609e67 : as_long:-4672 : from_prev:-64 : [c=a]
stack address:0x7fffb5609e27 : as_long:-4736 : from_prev:-64 : [c=a]
stack address:0x7fffb5609de7 : as_long:-4800 : from_prev:-64 : [c=a]
stack address:0x7fffb5609da7 : as_long:-4864 : from_prev:-64 : [c=a]
stack address:0x7fffb5609d67 : as_long:-4928 : from_prev:-64 : [c=a]
stack address:0x7fffb5609d27 : as_long:-4992 : from_prev:-64 : [c=a]
stack address:0x7fffb5609ce7 : as_long:-5056 : from_prev:-64 : [c=a]
stack address:0x7fffb5609ca7 : as_long:-5120 : from_prev:-64 : [c=a]
stack address:0x7fffb5609c67 : as_long:-5184 : from_prev:-64 : [c=a]
stack address:0x7fffb5609c27 : as_long:-5248 : from_prev:-64 : [c=a]
stack address:0x7fffb5609be7 : as_long:-5312 : from_prev:-64 : [c=a]
stack address:0x7fffb5609ba7 : as_long:-5376 : from_prev:-64 : [c=a]
stack address:0x7fffb5609b67 : as_long:-5440 : from_prev:-64 : [c=a]
stack address:0x7fffb5609b27 : as_long:-5504 : from_prev:-64 : [c=a]
stack address:0x7fffb5609ae7 : as_long:-5568 : from_prev:-64 : [c=a]
stack address:0x7fffb5609aa7 : as_long:-5632 : from_prev:-64 : [c=a]
stack address:0x7fffb5609a67 : as_long:-5696 : from_prev:-64 : [c=a]
stack address:0x7fffb5609a27 : as_long:-5760 : from_prev:-64 : [c=a]
stack address:0x7fffb56099e7 : as_long:-5824 : from_prev:-64 : [c=a]
stack address:0x7fffb56099a7 : as_long:-5888 : from_prev:-64 : [c=a]
stack address:0x7fffb5609967 : as_long:-5952 : from_prev:-64 : [c=a]
stack address:0x7fffb5609927 : as_long:-6016 : from_prev:-64 : [c=a]
stack address:0x7fffb56098e7 : as_long:-6080 : from_prev:-64 : [c=a]
stack address:0x7fffb56098a7 : as_long:-6144 : from_prev:-64 : [c=a]
stack address:0x7fffb5609867 : as_long:-6208 : from_prev:-64 : [c=a]
stack address:0x7fffb5609827 : as_long:-6272 : from_prev:-64 : [c=a]
stack address:0x7fffb56097e7 : as_long:-6336 : from_prev:-64 : [c=a]
stack address:0x7fffb56097a7 : as_long:-6400 : from_prev:-64 : [c=a]

*/
