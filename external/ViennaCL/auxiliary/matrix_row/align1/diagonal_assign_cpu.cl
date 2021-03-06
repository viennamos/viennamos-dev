
__kernel void diagonal_assign_cpu(
          __global float * A,
          unsigned int start1,          unsigned int start2,
          unsigned int inc1,            unsigned int inc2,
          unsigned int size1,           unsigned int size2,
          unsigned int internal_size1,  unsigned int internal_size2,
          float alpha)
{
  for (unsigned int row = get_global_id(0); row < size1; row += get_global_size(0))
    A[(row * inc1 + start1) * internal_size2 + row * inc2 + start2] = alpha;
}

